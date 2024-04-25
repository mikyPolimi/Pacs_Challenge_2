
#include <iostream>
#include "matrix.hpp"




int main(){
    using namespace algebra;
    

    using T = double;

    matrix<T,StorageOrder::row_wise> m;
    m(1,2) = 3;



    return 0;
}