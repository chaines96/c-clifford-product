#include "clifford_prod.h"

int collect_sign(unsigned a, unsigned b, float* squares) {
    int sign = 1; 
    int dimm = 0;
    while (a) {
        unsigned lowest = a & -a; // 'lowest' becomes equal to the lowest set bit of a/
        if (b & lowest) //a and b will "square" with eachother.
            sign = sign*squares[dimm];
        // lowest-1 will have 1's in each digit to the right of the lowest bit in a. Thus, 'b & (lowest-1)' will contain all k indeces between i and j- and thus we will need to perform k swaps.
        // __builtin_popcount is part of the gcc compiler and returns the amount of 1's in the input word.
        if (__builtin_popcount(b & (lowest - 1)) & 1)
            sign = -sign;
        a ^= lowest; //Remove the lowest bit from a for the next loop.
        dimm++;
    }
    return sign;
}

float* clifford_product(float *A, float *B, float *product, float* squares, int dim)
{
    unsigned limit = 1U << dim;
    //For each i in i...terms,, calculate the sign of a*b and multiply the coefficient at a^b by the product of the two coefficients times the sign.
    for (unsigned i = 0; i < limit;i++)
    {
        for (unsigned j = 0;j < limit;j++)
        {
            unsigned blade = i^j; //This is the index to which the clifford_product function will write the product to. The XOR cancels out the squared basis vectors.
            product[blade] += A[i]*B[j]*collect_sign(i,j,squares);
        }
    }
    return product;
}
