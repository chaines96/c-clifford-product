#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "clifford_prod.h"

int main() {
    int dimension = 1;
    printf("Enter the dimensions of your vector space (min 1, max 32)\n");
    fscanf(stdin,"%d",&dimension);

    float* squares = calloc(dimension, sizeof(float)); //Allocates an array of floats of size n
    for (int i = 0;i<dimension;i++)
    {
        printf("Enter the the square of the e%u.\n",(i+1));
        fscanf(stdin,"%f",(squares+i));
    }

    unsigned limit = 1U << dimension; //An n dimensional space can have possible 2^n multivectors without regards to order.
    float* A = calloc(limit, sizeof(float));
    float* B = calloc(limit, sizeof(float));

    printf("Enter the scalar component of the left multivector.\n");
    fscanf(stdin,"%f",(A));
    for (unsigned i=1;i<limit;i++)
    {
        printf("Enter coefficient for component");
        unsigned j = 1; int k = 1;
        while (j < limit)
        {
            if (j & i)
            {
                printf(" e%d",k);
            }
            j = j << 1;
            k += 1;
        }
        printf("\n");
        fscanf(stdin,"%f",(A+i));
    }

    printf("Enter the scalar component of the right multivector.\n");
    fscanf(stdin,"%f",(B));
    for (unsigned i=1;i<limit;i++)
    {
        printf("Enter coefficient for component");
        unsigned j = 1; int k = 1;
        while (j < limit)
        {
            if (j & i)
            {
                printf(" e%d",k);
            }
            j = j << 1;
            k += 1;
        }
        printf("\n");
        fscanf(stdin,"%f",(B+i));
    }
    float* C = calloc(limit, sizeof(float)); //Product
    clifford_product(A,B,C,squares,dimension);

    printf("The clifford product is:\n%.2f ",C[0]);

    for (unsigned i=1;i<limit;i++)
    {
        if (C[i])
        {
            if (C[i] > 1)
            {
                printf("+ %.2f ",C[i]);
            }
            else
            {
                printf("- %.2f ",(-C[i]));
            }
            unsigned j = 1; int k = 1;
            while (j < limit)
            {
                if (j & i)
                {
                    printf("e%d",k);
                }
                j = j << 1;
                k += 1;
            }
        }
    }
    printf("\n");
    free(A); free(B); free(C); free(squares);
    return 0;
}