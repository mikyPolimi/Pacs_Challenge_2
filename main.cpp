
#include <iostream>
#include "matrix.hpp"
//#include "ReadMatrix.hpp"
#include "chrono.hpp"

void test_mat_vec_prod(){
    using namespace algebra;
    using namespace Timings;
    using T = double;
    Chrono chrono;

    Matrix<T,StorageOrder::row_wise> A("lnsp_131.mtx");

    // generate vector of size A_num_columns
    std::vector<T> v(A.get_ncol(),1.0);
    std::vector<T> res;
    chrono.start();
    res = A*v;
    chrono.stop();
    std::cout<<"Row_wise dynamic operation: "<<chrono<<std::endl;
//
    A.compress();
    chrono.start();
    res = A*v;
    chrono.stop();
    std::cout<<"Row_wise compressed operation: "<<chrono<<std::endl;


    Matrix<T,StorageOrder::column_wise> B("lnsp_131.mtx");

   
    chrono.start();
    res = B*v;
    chrono.stop();
    std::cout<<"Column_wise dynamic operation: "<<chrono<<std::endl;
//
    B.compress();
    chrono.start();
    res = B*v;
    chrono.stop();
    std::cout<<"Column_wise compressed operation: "<<chrono<<std::endl;


};
int main(int argc, char **argv){

   // test_mat_vec_prod();
    using namespace algebra;
    using namespace Timings;

    //using T = std::complex<double>;
    using T = double;
    Matrix<T,StorageOrder::column_wise> A("lnsp_131.mtx");
    //std::cout<<A<<std::endl;
    A.compress();
    //std::cout<<A<<std::endl;
    A.uncompress();
    A.resize(5,5);
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
    M.resize(3,3);
    std::cout << "Stream operator:" << std::endl;
    std::cout << M;
    std::cout << std::endl;
/*
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