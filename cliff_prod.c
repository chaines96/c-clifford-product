#include <stdlib.h>
#include <string.h>

unsigned truncate(unsigned a, unsigned b, int left) {
    if (left)
    {
        while (!(a&1U))
        {
            a = a >> 1;
            b = b >> 1;
        }
    }
    else
    {
        unsigned int max = 1U << (sizeof(unsigned int) * 8 - 1);
        while (!(a&max))
        {
            a = a << 1;
            b = b << 1;
        }
    }
    return b; //The rightmost bits of b have been truncated to the leftmost in bit a.
}

int get_number_1s(unsigned a){
    int i = 0;
    while (a) //a != 0
    {
        if (!(a & 1U))
        {
            i++;
        }
        a = a >> 1;
    }
    return i;
}

int collect_sign (unsigned a, unsigned b) {
    unsigned squares = a&b;
    unsigned no_squares = a^b;
    truncate(squares,no_squares,0);
    truncate(squares,no_squares,1);
    if (get_number_1s(no_squares)%2) {return -1;} else {return 1;}
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

int main(int argc, char* argv[]) {
    float* A = malloc(sizeof(float)*argc);
    float* B = malloc(sizeof(float)*argc);
    int B_index = parse_blade(argc, argv, 0, A);
    parse_blade(argc, argv, B_index, B);
    int dim = argc - B_index;
    float* C = malloc(sizeof(float)*B_index);
    C = clifford_product(A, B, dim);
    free(A);
    free(B);
    free(C);
    return 0;
}
