#include <stdlib.h>
#include <math.h>

typedef struct k_blade_s {
    unsigned *basis_vector; //the i-th bit represents the i-th basis vector in the product.
    float coefficient;
} k_blade;

unsigned truncate_left(unsigned a, unsigned b) {
    while (!(a&1U))
    {
        a = a >> 1;
        b = b >> 1;
    }
    return b; //The rightmost bits of b have been truncated to the leftmost in bit a.
}

int get_left_1s(unsigned a){
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

int collect_sign (unsigned *a, unsigned *b, int dim) {
    unsigned squares;
    unsigned no_squares;
    int sign = 1;
    int flag = 1; //whether we've found our lowest bit or not.'
    for (int i = 0;i < dim;i++)
    {
        no_squares = a[i] ^ b[i]; //XOR the terms.
        squares = a[i] & b[i]; //squared terms only
        if (no_squares && flag)
        {
            unsigned no_squares = truncate_left(squares, no_squares);
            flag = 0;
        }
        if (!flag)
        {
            if (get_left_1s(no_squares) % 2) sign = -sign;
        }

    }
    return sign;
}

int main() {
    return 0;
}
