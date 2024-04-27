#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

#include "matrix.hpp"

namespace algebra{


// add a new element in position (i,j)
template<class T, StorageOrder S>
         T& Matrix<T,S>::operator ()(std::size_t i, std::size_t j){

            // handle the case where the element is already present
            assert(m_dyn_data.find({i,j}) ==  m_dyn_data.end());

            //update data:
            ++m_nnz;


            return m_dyn_data[{i,j}];
        }

// read the element in position (i,j)
template<class T, StorageOrder S>
    T& Matrix<T,S>::operator ()(std::size_t i, std::size_t j)const {
    if(m_dyn_data.find({i,j}) ==  m_dyn_data.end())
        return 0;
    return m_dyn_data.at({i,j});
}




template <class U, StorageOrder O>
        std::ostream &
        operator<<(std::ostream &stream, Matrix<U,O> &M){

            stream << "nrows = " << M.m_nrows << "; ncols = " << M.m_ncol
            << "; nnz = " << M.m_nnz << ";" << std::endl;

            if(M.m_is_compr){
                stream << "You are dealing with a compressed Matrix"<<std::endl;
                stream << "Values: ";
                for (const U& val : M.m_compr_data.values) {
                    stream << val << " ";
                }
                stream << std::endl;

                stream << "Row Indices: ";
                for (const num_type& idx : M.m_compr_data.row_idx) {
                    stream << idx << " ";
                }
                stream << std::endl;

                stream << "Column Indices: ";
                for (const num_type& idx : M.m_compr_data.col_idx) {
                    stream << idx << " ";
                }
                stream << std::endl;
            }


            else{
                stream << "You are dealing with a dynamic Matrix"<<std::endl;
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
    if(m_is_compr)
        return ;

    m_compr_data.resize(m_nrows,m_ncol,m_nnz);
    num_type iter = 0; // iterate all non zero elements (from 0 to m_nnz)
    num_type idx = 0; // index of the row/col (from 0 to m_nrows/n_cols)
    for(const auto& p : m_dyn_data){

        m_compr_data.values[iter] = p.second;
        num_type i = p.first[0];
        num_type j = p.first[1];
        // in row_wise case:
        if constexpr(S == StorageOrder::row_wise){
            m_compr_data.col_idx[iter] = j;
            if( i >= idx ) // when i reaches idx value it means we have reached a new row
                for ( num_type k = idx ; k <= i ; ++k){// i have to deal with the case one row is made of all zero elements
                    m_compr_data.row_idx[k] = iter;
                    ++idx;
                }
            m_compr_data.row_idx[m_nrows] = m_nnz;
        }

        // column_wise
        else{
            
            m_compr_data.row_idx[iter] = i;
            if( j >= idx ) // when i reaches idx value it means we have reached a new row
                for ( num_type k = idx ; k <= j ; ++k){// i have to deal with the case one row is made of all zero elements
                    m_compr_data.col_idx[k] = iter;
                    ++idx;
                }
            m_compr_data.col_idx[m_ncol] = m_nnz;
        }

        ++iter;
    }
    m_dyn_data.clear();
    m_is_compr = true;
}






    };



#endif