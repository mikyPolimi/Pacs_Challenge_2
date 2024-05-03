
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
    v[3] = 5;
    v[16] = 200;
    std::vector<T> res;
    chrono.start();
    std::vector<T> res1 = A*v;
    chrono.stop();
    std::cout<<"Row_wise dynamic operation: "<<chrono<<std::endl;
//
    A.compress();
    chrono.start();
    std::vector<T> res2 = A*v;
    chrono.stop();
    std::cout<<"Row_wise compressed operation: "<<chrono<<std::endl;

    Matrix<T,StorageOrder::column_wise> B("lnsp_131.mtx");

   
    chrono.start();
    std::vector<T> res3 = B*v;
    chrono.stop();
    std::cout<<"Column_wise dynamic operation: "<<chrono<<std::endl;
//
    B.compress();
    chrono.start();
    std::vector<T> res4 = B*v;
    chrono.stop();
    std::cout<<"Column_wise compressed operation: "<<chrono<<std::endl;

if(res1==res2)
    std::cout<<"ok_1"<<std::endl;;
if(res2==res3)
    std::cout<<"ok_2"<<std::endl;;
if(res3==res4)
    std::cout<<"ok_3"<<std::endl;;


};

void test_norm(){
    using namespace algebra;
    using T = std::complex<double>;



    Matrix<T,StorageOrder::row_wise> A("lnsp_131.mtx");
    Matrix<T,StorageOrder::column_wise> B("lnsp_131.mtx");
    //std::cout<<A<<std::endl;

    // 1 norm:

    std::cout<<"norm1 dynamic row : "<<A.norm<norm_type::One>()<<std::endl;
    A.compress();
    std::cout<<"norm1 compress row : "<<A.norm<norm_type::One>()<<std::endl;

    std::cout<<"norm1 dynamic col : "<<B.norm<norm_type::One>()<<std::endl;
    B.compress();
    std::cout<<"norm1 compress col : "<<B.norm<norm_type::One>()<<std::endl;
    std::cout<<std::endl;

    // infinity norm:

    A.uncompress();
    B.uncompress();
    Matrix<T,StorageOrder::row_wise> C("lnsp_131.mtx");
    Matrix<T,StorageOrder::column_wise> D("lnsp_131.mtx");

    std::cout<<"norm inf dynamic row : "<<A.norm<norm_type::Infinity>()<<std::endl;
    A.compress();
    //std::cout<<A<<std::endl;
    std::cout<<"norm inf compress row : "<<A.norm<norm_type::Infinity>()<<std::endl;

    std::cout<<"norm inf dynamic col : "<<B.norm<norm_type::Infinity>()<<std::endl;
    B.compress();
    std::cout<<"norm inf compress col : "<<B.norm<norm_type::Infinity>()<<std::endl;

    std::cout<<std::endl;

// Froebenius norm:
    
    A.uncompress();
    B.uncompress();

    std::cout<<"norm Froebenius dynamic row : "<<A.norm<norm_type::Frobenius>()<<std::endl;
    A.compress();
    //std::cout<<A<<std::endl;
    std::cout<<"norm Froebenius compress row : "<<A.norm<norm_type::Frobenius>()<<std::endl;

    std::cout<<"norm Froebenius dynamic col : "<<B.norm<norm_type::Frobenius>()<<std::endl;
    B.compress();
    std::cout<<"norm Froebenius compress col : "<<B.norm<norm_type::Frobenius>()<<std::endl;






    Matrix<T,StorageOrder::row_wise> M (4,5);
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
    std::cout<<"norminf dynamic row : "<<M.template norm<norm_type::Frobenius>()<<std::endl;
    M.compress();
    std::cout<<"norminf compress row : "<<M.template norm<norm_type::Frobenius>()<<std::endl;

    Matrix<T,StorageOrder::column_wise> M2 (4,5);
    M2(0, 2) = 3;
    M2(0, 4) = 4;
    M2(1, 2) = 5;
    M2(1, 3) = 7;
    M2(3, 1) = 2;
    M2(3, 2) = 6;
    M2(3, 2) = 8;
    std::cout<<"norminf dynamic col : "<<M2.template norm<norm_type::Frobenius>()<<std::endl;
    M2.compress();
    std::cout<<"norminf compress col : "<<M2.template norm<norm_type::Frobenius>()<<std::endl;

};


int main(){

    test_mat_vec_prod();
    //test_norm();
    using namespace algebra;
    using namespace Timings;
/*
    //using T = std::complex<double>;
    using T = double;
    Matrix<T,StorageOrder::row_wise> A("lnsp_131.mtx");
    //std::cout<<A<<std::endl;
    A.compress();
    //std::cout<<A<<std::endl;
    A.uncompress();
   
    A.resize(5,5);
    std::cout<<A<<std::endl;
    A.compress();
    std::cout<<A<<std::endl;


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
    //M.resize(3,3);

    M.compress();
    std::cout << M;
    std::cout << std::endl;
    M.uncompress();
    std::cout<<"after decompression: "<<std::endl;
    std::cout<<M<<std::endl;
    std::vector<T> v{1,1,1,1,2};
    std::vector<T> res = M*v;

    for (const auto& p : res)
        std::cout<<p<<std::endl;
   
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