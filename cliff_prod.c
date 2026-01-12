#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
int popcount(unsigned a) {
    int c = 0;
    while (a) {
        c += a & 1U;
        a >>= 1;
    }
    return c;
}*/

int collect_sign(unsigned a, unsigned b) {
    int sign = 1;
    while (a) {
        unsigned lowest = a & -a;          // lowest set bit in a
        a ^= lowest;
        if (__builtin_popcount(b & (lowest - 1)) & 1)
            sign = -sign;
    }
    return sign;
}

float* clifford_product(float *A, float *B, int dim)
{
    unsigned limit = 1U << dim;
    int csize = dim;
    float* product = calloc(csize, sizeof(float));
    //For each i in i...terms,, calculate the sign of a*b and multiply the coefficient at a^b by the product of the two coefficients times the sign.
    for (unsigned i = 0; i < limit;i++)
    {
        for (unsigned j = 0;j < limit;j++)
        {
            unsigned blade = i^j; //This is the index to which the clifford_product function will write the product to. The XOR cancels out the squared basis vectors.
            product[blade] += A[i]*B[j]*collect_sign(i,j);
        }
    }
    return product;
}


int main() {
    int dimension = 1;
    printf("Enter the dimensions of your vector space (min 1, max 32)\n");
    fscanf(stdin,"%d",&dimension);
    unsigned limit = 1U << dimension;
    float* A = calloc(dimension, sizeof(float));
    float* B = calloc(dimension, sizeof(float));
    for (unsigned i=0;i<limit;i++)
    {
        printf("Enter coefficient for component  %u of the left multivector.\n",i);
        fscanf(stdin,"%f",(A+i));
    }
    for (unsigned i=0;i<limit;i++)
    {
        printf("Enter coefficient for component  %u of the right multivector.\n",i);
        fscanf(stdin,"%f",(B+i));
    }
    float* C = clifford_product(A,B,dimension);
    printf("The clifford product is:\n%.2f ",C[0]);
    for (unsigned i=1;i<limit;i++)
    {
        printf("+ %.2f e%u",C[i],i);
    }
    free(A); free(B); free(C);
    return 0;
}
