#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

#include "matrix.hpp"

namespace algebra{    



    template<class T, StorageOrder S>
void Matrix<T,S>::readMatrixMarket(const std::string& filename) {
       std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    getline(file, line); // Read the first line to check for MatrixMarket format
    if (line.find("%%MatrixMarket matrix coordinate real general") == std::string::npos) {
        std::cerr << "Error: This file is not in MatrixMarket format." << std::endl;
        return;
    }

    // Read matrix size information
    do {
        getline(file, line);
    } while (line[0] == '%'); // Skip comment lines
    std::stringstream ss(line);
    idx_type nnz;
    ss >> m_nrows >> m_ncol >> nnz;

    // Read matrix elements and import as dynamic
    for (idx_type i = 0; i < nnz; ++i) {
        idx_type row, col;
        double value;
        file >> row >> col >> value;
        (*this)(row - 1,col - 1) = value; // Adjust for 0-based indexing
    }
    if(nnz!=m_nnz)
        std::cerr<<"Wrong number of non zeros elements stored! "<<std::endl;

    file.close();
}

    template<class T, StorageOrder S>
    void Matrix<T,S>::resize(idx_type row,idx_type col){
            if(!is_compressed()){
                if(row < m_nrows or col < m_ncol){
                    auto it = m_dyn_data.begin();
                    while( it != m_dyn_data.end() ) {
                        if(it->first[0]>=row or it->first[1]>=col)
                            it = m_dyn_data.erase(it);
                        else
                            ++it;
                    }
                }                
                m_nrows = row;
                m_ncol = col;
                update_nnz();
            }
            else
                std::cerr << "It is only possible to resize dynamic-stored matrix"<<std::endl;
        }


// add a new element in position (i,j)
    template<class T, StorageOrder S>
    T& Matrix<T,S>::operator ()(idx_type i, idx_type j){
       
        if( is_compressed() ){
            if(is_in_range(i,j)){
            
            m_compr_data.adjust_idx(i,j);
            
            idx_type r = m_compr_data.inner_idx[i];
            idx_type r_1 = m_compr_data.inner_idx[i+1];
            for(idx_type k = r ; k < r_1 ; ++k)
                if (m_compr_data.outer_idx[k] == j)
                    return m_compr_data.values[k];
            }
            // if it didnt return anything it means there is no value stored in position (i,j)
            std::cerr<<"this element does not exist"<<std::endl;
            static T ret;
            return ret;

        }

        else{ //dynamic storage
           
            // handle the case where the element is already present
            if (m_dyn_data.find({i,j}) !=  m_dyn_data.end())
                std::cout << "element already present, overwrite"<<std::endl;
            else{
            //update data:
            ++m_nnz;
            m_ncol = std::max(m_ncol,j);
            m_nrows = std::max(m_nrows,i);
            }
            return m_dyn_data[{i,j}];
        }
    }

    // read the element in position (i,j)
    template<class T, StorageOrder S>
    T& Matrix<T,S>::operator ()(idx_type i, idx_type j)const {
        assert(is_in_range(i,j));

        if(is_compressed()){ // compressed case
            m_compr_data.adjust_idx(i,j);

            idx_type r = m_compr_data.inner_idx[i];
            idx_type r_1 = m_compr_data.inner_idx[i+1];
            for(idx_type k = r ; k < r_1 ; ++k)
                if (m_compr_data.outer_idx[k] == j)
                    return m_compr_data.values[k];
            }

        else // dynamic storage
            if( m_dyn_data.find({i,j}) !=  m_dyn_data.cend())
                return m_dyn_data.at({i,j});

        return 0;
    }

// method to compress the matrix
template<class T, StorageOrder S>
void Matrix<T,S>::compress(){

    // if matrix already compressed exit
    if(is_compressed()){
        std::cerr << "matrix already compressed" << std::endl;
        return ;
    }

    // resize the compressed container
    idx_type inner_len;
    S==StorageOrder::row_wise? inner_len=m_nrows+1 : inner_len=m_ncol+1;
    update_nnz(); 
    m_compr_data.values.resize(m_nnz); // it is more efficient than doing every time a push back of a vector
    m_compr_data.outer_idx.resize(m_nnz);
    m_compr_data.inner_idx.resize(inner_len);


    idx_type iter = 0; // iterate all non zero elements (from 0 to m_nnz)
    idx_type idx = 0; // index of the row/col (from 0 to m_nrows/n_cols)
    for(const auto& p : m_dyn_data){
        m_compr_data.values[iter] = p.second;
        idx_type in = p.first[0];
        idx_type out = p.first[1];

        m_compr_data.adjust_idx(in,out); // in column_wise ordered invert the indexes
        
        m_compr_data.outer_idx[iter] = out;
        if( in >= idx ) // when in reaches idx value it means we have reached a new row (row_ordered)
            for ( idx_type k = idx ; k <= in ; ++k){// deal with the case one row is made of all zero elements
                m_compr_data.inner_idx[k] = iter;
                ++idx;
            }

        ++iter;
    }
    m_compr_data.inner_idx[inner_len-1] = m_nnz;

    m_dyn_data.clear();
    m_is_compr = true;
}


// method to uncompress the matrix storage
template<class T, StorageOrder S>
void Matrix<T,S>::uncompress(){

    // clear the dynamic container
    m_dyn_data.clear();

    idx_type iter = 0;
    
    for (idx_type r = 0 ; r < m_compr_data.inner_idx.size()-1; ++r)
        for(idx_type i = m_compr_data.inner_idx[r]; i < m_compr_data.inner_idx[r+1] ; ++i){
            m_dyn_data[{r,m_compr_data.outer_idx[iter]}] = m_compr_data.values[iter];
            ++iter;
        }

    m_is_compr = false;
    m_compr_data.clear();
}


// stream operator
template <class U, StorageOrder O>
std::ostream& operator<<(std::ostream &stream, Matrix<U,O> &M){

    stream << "nrows = " << M.m_nrows << "; ncols = " << M.m_ncol
    << "; nnz = " << M.m_nnz << ";" << std::endl;
    stream << "You are dealing with a ";
    M.is_row_wise() ? (stream << "row_wise") : (stream << "column_wise");
    if(M.m_is_compr){
            stream <<" compressed Matrix"<<std::endl;
        stream << "Values: ";
        for (const U& val : M.m_compr_data.values) {
            stream << val << " ";
        }
        stream << std::endl;

        stream << "Inner Indices: ";
        for (const idx_type& idx : M.m_compr_data.inner_idx) {
            stream << idx << " ";
        }
        stream << std::endl;

        stream << "Outer Indices: ";
        for (const idx_type& idx : M.m_compr_data.outer_idx) {
            stream << idx << " ";
        }
        stream << std::endl;
    }


    else{
        stream << " dynamic Matrix"<<std::endl;
        stream << "mat = [ " << std::endl;
        for (const auto& p : M.m_dyn_data)
            {
                stream << p.first[0] << ", " << p.first[1] << ", ";
                stream << std::setprecision(16) << p.second << ";" << std::endl;
            }
        stream << "];" << std::endl;
    }

    return stream;
    
    }





// performing Av = b
        template <class U, StorageOrder O>
        std::vector<U> operator* (const Matrix<U,O> &M,const std::vector<U>& v){

            //check compatible sizes
            if(M.m_ncol == v.size()){

                std::vector<U> res(M.m_nrows,0); // result vector

                if(M.is_compressed()){
                    idx_type iter = 0;
                    if constexpr(O == StorageOrder::row_wise)
                        for (idx_type r = 0 ; r < M.m_compr_data.inner_idx.size()-1; ++r)
                            for(idx_type i = M.m_compr_data.inner_idx[r]; i < M.m_compr_data.inner_idx[r+1] ; ++i, ++iter)
                                res[r] += M.m_compr_data.values[iter] * v[M.m_compr_data.outer_idx[iter]];
                                
                               // m_dyn_data[{r,m_compr_data.outer_idx[iter]}] = m_compr_data.values[iter];
                                                   
                    else
                        for (idx_type c = 0 ; c < M.m_compr_data.inner_idx.size()-1; ++c)
                            for(idx_type i = M.m_compr_data.inner_idx[c]; i < M.m_compr_data.inner_idx[c+1] ; ++i, ++iter)
                                res[M.m_compr_data.outer_idx[iter]] += M.m_compr_data.values[iter] * v[c];

                }
                else // dynamic storage
                    for(const auto& m : M.m_dyn_data)
                        res[m.first[0]] += m.second*v[m.first[1]];

                return res;
            }
            
            // wrong size
            std::cerr << "Unable to perform the operation, wrong size"<<std::endl;
            static std::vector<U> ret;
            return ret;
        }
/*
        // Extend the matrix vector operator to accept also as vector a Matrix with just one column
        template <class U, StorageOrder O>
        std::vector<U> operator* (const Matrix<U,O> & M,const Matrix<U,O> & v){
            assert ( v.m_ncol==1 and v.m_nrows == M.m_ncol);
                // copy the matrix inside a vector 

//! TODO
        }
        */

};





#endif