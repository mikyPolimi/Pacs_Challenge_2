
#include <iostream>
#include "matrix.hpp"




int main(){
    using namespace algebra;
    

    using T = double;


 


    Matrix<T,StorageOrder::row_wise> m (2,2);
    m(1,1) = 3;

    std::cout << "Stream operator:" << std::endl;
    std::cout << m;
    std::cout << std::endl;
    
    m.compress();



    return 0;
}