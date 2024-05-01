
#include <iostream>
#include "matrix.hpp"




int main(){
    using namespace algebra;
    

    using T = double;


 


    Matrix<T,StorageOrder::row_wise> M (4,5);
    M(0, 2) = 3;
    M(0, 4) = 4;
    M(1, 2) = 5;
    M(1, 3) = 7;
    M(3, 1) = 2;
    M(3, 2) = 6;
    M(3, 2) = 8;
    M.resize(2,2);
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;

    M.compress();
    
    std::cout << M << std::endl;
    M(3,2) = 5;
    
    std::cout<<M(3,2)<<std::endl;
    M.uncompress();
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;

    return 0;
}