
#include <iostream>
#include "matrix.hpp"
#include "ReadMatrix.hpp"



int main(){
    using namespace algebra;
    

    //using T = std::complex<double>;
    using T = double;
    Matrix<T,StorageOrder::column_wise> A("lnsp_131.mtx");
    std::cout<<A<<std::endl;
    A.compress();
    std::cout<<A<<std::endl;

/*
    Matrix<T,StorageOrder::column_wise> M (4,5);
    M(0, 2) = 3;
    M(0, 4) = 4;
    M(1, 2) = 5;
    M(1, 3) = 7;
    M(3, 1) = 2;
    M(3, 2) = 6;
    M(3, 2) = 8;
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;
    //M.resize_mat(2,2);
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;

   // M.compress();
     std::cout << M;
    std::cout << std::endl;
    std::vector<T> v{1,1,1,1,2};
    std::vector<T> res = M*v;

    for (const auto& p : res)
        std::cout<<p<<std::endl;
    std::cout << M << std::endl;
   
    /*
     M(3,2) = 5;
    std::cout<<M(3,2)<<std::endl;
    M.uncompress();
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;
*/
    return 0;
}