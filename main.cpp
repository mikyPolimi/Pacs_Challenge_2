
#include <iostream>
#include "matrix.hpp"




int main(){
    using namespace algebra;
    

    using T = double;


 


    Matrix<T,StorageOrder::column_wise> M (4,5);
    M(0,2) = 3;
    M(0, 4) = 4;
    M(1, 2) = 5;
    M(1, 3) = 7;
    M(3, 1) = 2;
    M(3, 2) = 6;

    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;

    M.compress();

    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;

    return 0;
}