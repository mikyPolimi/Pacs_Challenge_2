
# PACS_Challenge_2

This repository contains the second challenge of the course "Advanced Programming for Scientific Computing" at Politecnico di Milano.

## Introduction

The goal of this project is to develop a library that deals with sparse matrices. Sparse matrices are matrices with a significant number of zero elements compared to non-zero elements. They find applications in various scientific and engineering fields where memory and computational efficiency are crucial.

In this project, we explore different storage techniques for sparse matrices, including dynamic (uncompressed) and compressed storage methods. Dynamic techniques allow for easy addition or deletion of non-zero elements, while compressed techniques offer superior memory and computational efficiency for basic operations like matrix-vector product.

We have implemented the following storage techniques:

- **COOmap**: Dynamic storage using a map (i, j) → Aij, where the pair (i, j) acts as the key. Different orderings may be used for logical ordering column-wise or row-wise.

- **CSR (Compressed Sparse Row)**: Efficient compressed storage where two index vectors are used to store row-wise data.

- **CSC (Compressed Sparse Column)**: Similar to CSR but stores data column-wise.

## Most Useful Available Methods

- `compress()` and `uncompress()`: Methods to switch the internal storage of the matrix.

- `operator ()`: Both const and non-const versions to access elements of the matrix.

- `operator *`: Overloaded for multiplication of matrices.

- `norm()`: Template method to calculate various norms such as One-norm, Infinity-norm, or Frobenius-norm.

- Constructor: Initializes the matrix using a file downloaded from *math.nist.gov/MatrixMarket/*.

## Implementation

Inside the `algebra` namespace:
- The template `Storage order S`, given by an enumerator.
- `Matrix`: A template class that represents the sparse matrix. Its input parameters are:
  - `T`: A class (e.g., double or std::complex).
  - `S`: An enumerator representing the order in which the matrix is traversed and stored: row-wise or column-wise.

Inside the `Matrix` class as private members:
- `dynamic_container<T,S> m_dyn_data`: Defined by a map: (i,j) -> elem.
- `Compressed_struct<T,S> m_compr_data`: A struct formed by 3 vectors, as explained before. It also has a useful method `adjust_idx(idx_type& in, idx_type& out)` which swaps the indexes in case the matrix is stored in column-wise order.

**Note:** The map used in `dynamic_container` is constructed through a custom comparator. This comparator is designed to overwrite the `<` operator in case the matrix is stored in column-wise order, ensuring correct ordering and efficient access to elements.


To test the code, two tests are executed in the `main` function:

1. `test_mat_vec_prod()`: Performs matrix-vector product and prints the execution time of the operations using all possible storage methods.

2. `test_norm()`: Calculates the one-norm, infinity-norm, and Frobenius-norm with all possible storage methods.

The make file has optimization activated using the -O3 flag.

Note: The implementation works fine with all possible types of classes as values of the map. Complex numbers can also be used.

### How to Use

To use this project, follow these steps:

1. Clone the repository:
   ```bash
   $ git clone https://github.com/mikyPolimi/PACS_Challenge_2.git
   $ cd PACS_Challenge_2
2. Run the code:
    ```bash
    $ make
    $ ./main
