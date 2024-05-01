#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

#include "matrix.hpp"

namespace algebra{    


// add a new element in position (i,j)
    template<class T, StorageOrder S>
    T& Matrix<T,S>::operator ()(std::size_t i, std::size_t j){
        std::cout<<"non const method"<<std::endl;
        
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
    T& Matrix<T,S>::operator ()(std::size_t i, std::size_t j)const {
        std::cout<<"const method"<<std::endl;
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
            if( m_dyn_data.find({i,j}) !=  m_dyn_data.end())
                return m_dyn_data.at({i,j});

        return 0;
    }




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
    m_compr_data.values.resize(m_nnz);
    m_compr_data.outer_idx.resize(m_nnz);
    m_compr_data.inner_idx.resize(inner_len);


    idx_type iter = 0; // iterate all non zero elements (from 0 to m_nnz)
    idx_type idx = 0; // index of the row/col (from 0 to m_nrows/n_cols)
    for(const auto& p : m_dyn_data){

        m_compr_data.values[iter] = p.second;
        idx_type in;
        idx_type out;
        if constexpr(S == StorageOrder::row_wise){// in row_wise ordered:
            in = p.first[0];
            out = p.first[1];
        }
        else{ // column_wise ordered
            in = p.first[1];
            out = p.first[0]; 
        }

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

    size_t iter = 0;
    
    for (idx_type r = 0 ; r < m_compr_data.inner_idx.size()-1; ++r)
        for(idx_type i = m_compr_data.inner_idx[r]; i < m_compr_data.inner_idx[r+1] ; ++i){
            m_dyn_data[{r,m_compr_data.outer_idx[iter]}] = m_compr_data.values[iter];
            ++iter;
        }

    m_is_compr = false;
    m_compr_data.clear();
}



};



#endif