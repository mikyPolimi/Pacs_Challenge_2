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

    
/*
    // struct were i store all the data for the dynamic version of the matrix
    template<StorageOrder S >
    class Position{
        public:
        std::array<std::size_t,2> pos;
        
        inline bool operator < (const Position& p1, const Position& p2){
            return p1 < p2;
        }

// partial specialization of the operator <
        template<>
        inline bool Position<StorageOrder::column_wise> operator < (const position& p1, const position& p2){
            return std::tie(p1.pos[1],p1.pos[0]) < std::tie(p2.pos[1],p2.pos[0]);
        }
    };
*/

    using position = std::array<std::size_t,2>;

struct custom_comparer
{
    bool operator()(const position& left, const position& right) const
    {      
        return left[1] < right[1] or (left[1] == right[1] and left[0] < right[0]);
    }
};

    template<class T, StorageOrder S>
    using dynamic_container = std::map<position,T>;

        //Dynamic_struct()

template<class T>
        using vec_type = std::vector<T>;

        using num_type = long unsigned int;
    // struct were I store all the data for the compressed version of the matrix

    template< class T, StorageOrder S >
    struct Compressed_struct{

        vec_type<T> values;
        vec_type<num_type> row_idx;
        vec_type<num_type> col_idx;

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






// matrix class

    template<class T,StorageOrder S>
    class Matrix{

    private:
        dynamic_container<T,S> m_dyn_data;
        Compressed_struct<T,S> m_compr_data;

        bool m_is_compr = false;

        num_type m_nnz = 0;
        num_type m_nrows = 0;
        num_type m_ncol = 0;




    public:
        Matrix() = default;
        Matrix(num_type row, num_type col) : 
        m_nrows(row), m_ncol(col) {};

        // add a new element in position (i,j)
        T& operator ()(std::size_t i, std::size_t j);

        // read the element in position (i,j)
        T& operator ()(std::size_t i, std::size_t j)const;


        //template<class T, StorageOrder S>
        //inline bool operator < (const vec_type& v1, const vec_type& v2)const {
          //  return v1 < v2;
        //}

        inline bool is_row_wise() const{
            return S == StorageOrder::row_wise;
        }



        template <class U, StorageOrder O>
        friend std::ostream &
        operator<<(std::ostream &stream, Matrix<U,O> &M);
           

        void compress();



    };




// occhio nel caso compressed non devo fare operazioni quindi non mi serve inheritance


// caso row default, traversing by column cambio la def di operatore <


};

#include "matrix_impl.hpp"

#endif