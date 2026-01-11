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
    float* product = calloc(dim, sizeof(float));
    //For each i in i...terms,, calculate the sign of a*b and multiply the coefficient at a^b by the product of the two coefficients times the sign.
    for (unsigned i = 0; i < dim;i++)
    {
        for (unsigned j = 0;j < dim;j++)
        {
            unsigned blade = i^j; //This is the index to which the clifford_product function will write the product to. The XOR cancels out the squared basis vectors.
            product[blade] += A[i]*B[j]*collect_sign(i,j);
        }
    }
    return product;
}

int parse_blade(int argc, char* argv[], int index, float* multivector)
{
    unsigned mask;
    int i;
    float coefficient;
    float tens_spot;
    for (i = index; i < argc;i++)
    {
        tens_spot = 1;
        mask = 0;
        int sign = 1;
        for (int j = 0; j < strlen(argv[i]);j++)
        {
            if (argv[i][j] == ')')
            {
                i++;
                return i; //end of current vector reached.
            }
            else
            {
                if (argv[i][j] == '-')
                {
                    sign = -sign;
                }
                else
                {
                    coefficient = argv[i][j]*tens_spot;
                    tens_spot = tens_spot*10;
                }
            }
            if (argv[i][j] == 'e')
            {
                int vector_index = argv[i][j];
                while((argv[i][j+1] != '\n') && (argv[i][j] < 9)) //while this character is numberic
                {
                    j++;
                    mask = mask & (1U << vector_index); //Adding 1 to the index
                }
            }
        }
        multivector[mask] = coefficient * sign; //Finally, we can discovered the coefficient of this statement and can move on.
    }
    return i;
}

float* parse_blade_inp(char* line)
{
    unsigned j = 0;
    char buffer[100];
    fgets(buffer, 100, stdin);
    int dimension = 1;
    float* multivec = calloc(20,sizeof(float));
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

int main(int argc, char* argv[]) {
    /*
    printf("Please enter your first vector.\n");
    float* A = parse_blade_inp(" ");
    printf("Please enter the second vector.\n");
    float* B = parse_blade_inp(" ");
    float* C = clifford_product(A, B, argc);
    free(A);
    free(B);
    free(C);*/
    unsigned a = 3;
    unsigned b = 1;
    if (collect_sign(a,b) > 0)
    {
        printf("Sign is positive! \n");
    }
    else
    {
        printf("Sign is negative! \n");
    }
    return 0;
}
