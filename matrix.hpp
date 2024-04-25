#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <map>
#include <array>






namespace algebra{


    enum class StorageOrder{
        row_wise,
        column_wise
    };


    template<class T>
    using container = std::map<std::array<std::size_t,2>,T>;


    template<class T,StorageOrder>
    class matrix{
    public:
        matrix() = default;
        inline T& operator ()(std::size_t i, std::size_t j){
            return m_data[{i,j}];
        }
    private:
        container<T> m_data;
    };



}



#endif