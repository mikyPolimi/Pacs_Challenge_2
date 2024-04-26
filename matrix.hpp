#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include<vector>
#include <array>
#include <cassert>





namespace algebra{


    enum class StorageOrder{
        row_wise,
        column_wise
    };

    using position = std::array<std::size_t,2>;

    template<class T>
    using dynamic_container = std::map<position,T>;

    template<class T>
    using vec_type = std::vector<T>;

    using num_type = long unsigned int;









    template<class T,StorageOrder>
    class matrix{
    public:
        matrix() = default;
        inline T& operator ()(std::size_t i, std::size_t j){

            // handle the case where the element is already present
            assert(m_data.find({i,j}) ==  m_data.end)
                return m_data[{i,j}];
        }

        inline void compress(){
            m_v.resize(m_nnz);
            if(StorageOrder == StorageOrder::row_wise){
                m_col_idx.resize(m_nnz);
                m_row_idx.resize(m_nrows+1,m_ncol); // initialize everything to m_ncol in case there are empty rows. Moreover the last element must be m_ncol
            }
            else{
                m_col_idx.resize(m_ncol+1);
                m_row_idx.resize(m_nnz);
            }
            num_type iter = 0; // from 0 to m_nnz
            num_type idx = 0; // from 0 to m_nrows or n_cols
            for(const auto& p : m_data){
                m_v[iter] = p.second;
                if(StorageOrder == StorageOrder::row_wise){
                m_col_idx[iter] = p.first[1];
                if(p.first[0] > idx-1){
                   m_row_idx[p.first[0]] = iter;
                   ++idx;
                }
                
                }

                else{
                    
                    m_row_idx[iter] = p.first[0];
                }

                ++iter;
            }

            m_data.clear;
            m_compr = true;
        }

    private:
        dynamic_container<T> m_data;
        vec_type<T> m_v;
        vec_type<T> m_row_idx;
        vec_type<T> m_col_idx;
        bool m_compr = false;

        num_type m_nnz = 0;
        num_type m_nrows = 0;
        num_type m_ncol = 0;



    };
// penso che la cosa migliore sia andare di inheritance:
// dynamic e sotto coo etc
// static e sotto csr etc


// occhio nel caso compressed non devo fare operazioni quindi non mi serve inheritance


// caso row default, traversing by column cambio la def di operatore <


}



#endif