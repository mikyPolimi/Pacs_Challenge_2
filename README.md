# Pacs_Challenge_2
Repository for the PACS second challenge
This project is a part of the Advanced Programming for Scientific Computing course at Politecnico di Milano. 
The goal of the project is to develop a library that deals with sparse matrix;
a matrix whose number of non-zero elements grows as N, being N the linear size
of the matrix (for instance the square root of the product of the number of rows times
the number of columns).

The way to store a sparse matrix is not unique, and we can distinguish between two
families of storage techniques:

**Dynamic** (or uncompressed) storage techniques. They allow to add (and sometimes
also eliminate) new non-zero elements easily. 

**Compressed** storage techniques. They are the most efficient in terms of memory
and of the computational efficiency of basic operations like matrix-vector product. But
they do not allow to change the pattern of sparsity.

In this project I developed as dynamic stored:

`COOmap`: where the map (i, j) → Aij is performed with a std::map where the couple i, j acts as the key. Different orderings may
be used so that the elements are logically ordered column-wise or row-wise. In other
words, with the appropriate ordering traversing the map provides the matrix elements
”by row” or ”by column”.


As compressed stored:

`CSR`: Compressed Sparse Row. We store two vectors of indexes. The first (the inner indexes),
of length the number of rows plus one, contains the starting index for the elements of
each row. The second vector of indexes (the outer indexes), of length the number of nonzeroes,
contains the corresponding column index. Finally, we have a vector of values,
again of length the number of non-zeroes. To be more specific, the elements of row i are
stored in the elements of the vector of values in the interval inner(i) ≤ k < inner(i + 1)
(the interval is open on the right) and the corresponding column index is outer(k).
Using this scheme, we have a row-wise storage, since transversing the vector of values
provides the non-zero elements ”by row”.

`CSC`: Compressed Sparse Column. As before, but with the role of row and column exchanged.
The matrix is thus stored column-wise.

# Avaiable methods
`compress()` and `uncompress()` able to switch the internal storage of the matrix

`is_compressed()`

Call operator `()`, both const and non const version

A friend `operator *` for the multiplication of the matrix

A template method `norm()` that takes as template value an enumerator
that may be:
`One`, `Infinity` or `Frobenius`.

A constructor of the matrix, given as input the name of the file downloaded from *math.nist.gov/MatrixMarket/*


## How to use

To use this project, follow these steps:

1. Clone the repository:
   ```bash
   $ git clone https://github.com/mikyPolimi/PACS_Challenge_2.git
   $ cd PACS_Challenge_2
2. Run the code:
    ```bash
    $ make
    $ ./main