#ifndef MATRIX_IMPL_HPP
#define MATRIX_IMPL_HPP

#include "matrix.hpp"


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>


namespace algebra{
// Function to read matrix from MatrixMarket format
template<class T, StorageOrder S>
void Matrix<T,S>::readMatrixMarket(const std::string& filename) {
       std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    getline(file, line); // Read the first line to check for MatrixMarket format
    if (line.find("%%MatrixMarket matrix coordinate real general") == std::string::npos) {
        std::cerr << "Error: This file is not in MatrixMarket format." << std::endl;
        return;
    }

    // Read matrix size information
    //int numRows, numCols, numNonZeros;
    do {
        getline(file, line);
    } while (line[0] == '%'); // Skip comment lines
    std::stringstream ss(line);
    ss >> m_nrows >> m_ncol >> m_nnz;

    // Resize matrix
    //matrix.resize(numRows, vector<double>(numCols, 0.0));

    // Read matrix elements
    for (idx_type i = 0; i < m_nnz; ++i) {
        idx_type row, col;
        double value;
        file >> row >> col >> value;
        (*this)(row - 1,col - 1) = value; // Adjust for 0-based indexing
    }

    file.close();
}
}
/*
int main() {
    vector<vector<double>> matrix;
    readMatrixMarket("matrix.mtx", matrix);

    // Display matrix
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            cout << elem << "\t";
        }
        cout << endl;
    }

    return 0;
}
*/
#endif