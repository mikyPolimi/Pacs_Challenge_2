
#include <iostream>
#include "matrix.hpp"
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
    v[20] = -98.351;

    std::cout<<"Matrix-Vetor computation:"<<std::endl;
    
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

    std::cout<<std::endl;
    std::cout<<"Matrix-Matrix computation:"<<std::endl;

    Matrix<T,StorageOrder::row_wise> V(A.get_ncol(),1);
    for(idx_type i = 0; i < v.size(); ++i)
        if ( v[i] != 0)
                V(i,0) = v[i];
    
    chrono.start();
    Matrix<T,StorageOrder::row_wise> res5 = A*V;
    chrono.stop();
    std::cout<<"Row_wise dynamic operation: "<<chrono<<std::endl;

};

void test_norm(){
    using namespace algebra;
    using T = std::complex<double>;



    Matrix<T,StorageOrder::row_wise> A("lnsp_131.mtx");
    Matrix<T,StorageOrder::column_wise> B("lnsp_131.mtx");
    

    // 1 norm:
    std::cout << "One norm: "<<std::endl;
    std::cout<<"norm1 dynamic row : "<<A.norm<norm_type::One>()<<std::endl;
    A.compress();
    std::cout<<"norm1 compress row : "<<A.norm<norm_type::One>()<<std::endl;

    std::cout<<"norm1 dynamic col : "<<B.norm<norm_type::One>()<<std::endl;
    B.compress();
    std::cout<<"norm1 compress col : "<<B.norm<norm_type::One>()<<std::endl;
    std::cout<<std::endl;


    // infinity norm:
    std::cout << "Infinity norm: "<<std::endl;
    A.uncompress();
    B.uncompress();
    

    std::cout<<"norm inf dynamic row : "<<A.norm<norm_type::Infinity>()<<std::endl;
    A.compress();
    std::cout<<"norm inf compress row : "<<A.norm<norm_type::Infinity>()<<std::endl;

    std::cout<<"norm inf dynamic col : "<<B.norm<norm_type::Infinity>()<<std::endl;
    B.compress();
    std::cout<<"norm inf compress col : "<<B.norm<norm_type::Infinity>()<<std::endl;

    std::cout<<std::endl;

// Froebenius norm:
    std::cout << "Froebenius norm: "<<std::endl;
    A.uncompress();
    B.uncompress();

    std::cout<<"norm Froebenius dynamic row : "<<A.norm<norm_type::Frobenius>()<<std::endl;
    A.compress();
    //std::cout<<A<<std::endl;
    std::cout<<"norm Froebenius compress row : "<<A.norm<norm_type::Frobenius>()<<std::endl;

    std::cout<<"norm Froebenius dynamic col : "<<B.norm<norm_type::Frobenius>()<<std::endl;
    B.compress();
    std::cout<<"norm Froebenius compress col : "<<B.norm<norm_type::Frobenius>()<<std::endl;


};


int main(){

    test_mat_vec_prod();
    test_norm();

    return 0;
}