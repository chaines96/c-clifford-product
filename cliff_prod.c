#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int popcount(unsigned a) {
    int c = 0;
    while (a) {
        c += a & 1U;
        a >>= 1;
    }
    return c;
}

int collect_sign(unsigned a, unsigned b) {
    int sign = 1;
    while (a) {
        unsigned lowest = a & -a;          // lowest set bit in a
        a ^= lowest;
        if (popcount(b & (lowest - 1)) & 1)
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

float* parse_blade_inp(char* line)
{
    int csize = 4;
    unsigned j = 0;
    char buffer[100];
    fgets(buffer, 100, stdin);
    int dimension = 1;
    float* multivec = calloc(csize,sizeof(float));
    char* token = strtok(buffer," ");
    while(token)
    {
        float candidate = 0;
        int tenspot = 1;
        int mode = 0;
        for (int i = 0;buffer[i];i++)
        {
            if (buffer[i] == 'e')
            {
                mode = 1;
                tenspot = 1;
            }
            else
            {
                if (mode)
                {
                    j = j & (1U << buffer[i]);
                }
                else
                {
                    if (buffer[i] == '-')
                    {
                        tenspot -= -1;
                    }
                    else
                    {
                        candidate += buffer[i]*tenspot;
                        tenspot *= 10;
                    }
                }
            }
        }
        dimension = 2*dimension;
        multivec[j] = candidate;
        token = strtok(NULL," ");
    }
    return multivec;
}

int main() {
    int dimension = 2;
    float* A = calloc(csize, sizeof(float));
    float* B = calloc(csize, sizeof(float));
    A[0] = 1; A[1] = 1;
    B[3] = 1;
    float* C = clifford_product(A,B,dimension);
    printf("Clifford product is %f + %f e1 + %f e2 + %f e1 e2\n",C[0],C[1],C[2],C[3]);
    free(A); free(B); free(C);
    return 0;
}
