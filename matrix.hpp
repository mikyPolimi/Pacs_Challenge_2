#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include<vector>
#include <array>
#include <cassert>
#include <iostream>
#include <iomanip>



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



    // struct were I store all the data for the compressed version of the matrix

    template< class T, StorageOrder S >
    struct Compressed{
        vec_type<T> values;
        vec_type<T> row_idx;
        vec_type<T> col_idx;

        inline void resize(num_type nrow, num_type ncol, num_type nnz){
            values.resize(nnz);

            if(S == StorageOrder::row_wise){
                col_idx.resize(nnz);
                row_idx.resize(nrow+1,ncol); // initialize everything to m_ncol in case there are empty rows. Moreover the last element must be m_ncol
            }
            else{
                col_idx.resize(ncol+1);
                row_idx.resize(nnz);
            }
        }
    };





    template<class T,StorageOrder S>
    class Matrix{
    public:
        Matrix() = default;
        Matrix(num_type row, num_type col) : 
        m_nrows(row), m_ncol(col) {};

        inline T& operator ()(std::size_t i, std::size_t j){

            // handle the case where the element is already present
            assert(m_dyn_data.find({i,j}) ==  m_dyn_data.end());

            //update data:
            ++m_nnz;


            return m_dyn_data[{i,j}];
        }

        inline bool is_row_wise() const{
            return S == StorageOrder::row_wise;
        }



        template <class U, StorageOrder O>
        friend std::ostream &
        operator<<(std::ostream &stream, Matrix<U,O> &M){
            stream << "nrows = " << M.m_nrows << "; ncols = " << M.m_ncol
            << "; nnz = " << M.m_nnz << ";" << std::endl;
            stream << "mat = [ \n";
            for (const auto& p : M.m_dyn_data)
                {
                    stream << p.first[0] << ", " << p.first[1] << ", ";
                    stream << std::setprecision(16) << p.second << ";" << std::endl;
                }
            stream << "];" << std::endl;
            return stream;
        }


        inline void compress(){
            m_compr_data.resize(m_nrows,m_ncol,m_nnz);
            num_type iter = 0; // from 0 to m_nnz
            num_type idx = 0; // from 0 to m_nrows or n_cols
            for(const auto& p : m_dyn_data){
                m_compr_data.values[iter] = p.second;
                if(is_row_wise()){
                    m_compr_data.col_idx[iter] = p.first[1];
                    if(p.first[0] > idx-1){
                        m_compr_data.row_idx[p.first[0]] = iter;
                        ++idx;
                }

                }
                

                // column_wise
                else{
                    
                    m_compr_data.row_idx[iter] = p.first[0];
                }

                ++iter;
            }

            m_dyn_data.clear();
            m_is_compr = true;
        }







    private:
        dynamic_container<T> m_dyn_data;
        Compressed<T,S> m_compr_data;

        bool m_is_compr = false;

        num_type m_nnz = 0;
        num_type m_nrows = 0;
        num_type m_ncol = 0;



    };




// occhio nel caso compressed non devo fare operazioni quindi non mi serve inheritance


// caso row default, traversing by column cambio la def di operatore <


}



#endif