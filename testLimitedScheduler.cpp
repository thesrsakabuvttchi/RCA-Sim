#include"LimitedScheduler.h"

int main()
{
    int size = 3;
    BaseScheduler Bs(size);
    
    int* A = new int[size * size];
    int* B = new int[size * size];
    int* C = new int[size * size];

    // Fill matrices A and B with random values (for demonstration purposes)
    for (int i = 0; i < size*size; ++i) {
        A[i] = i;
        B[i] = i;
    }

    std::cout<<"Matrix Multiplication Outputs:\n";
    Bs.scheduleMM(A,B,C);

    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            std::cout<<A[i*size+j]<<"\t";
        }
        std::cout<<"\t";
        for(int j=0;j<size;j++)
        {
            std::cout<<B[i*size+j]<<"\t";
        }
        std::cout<<"\t";
        for(int j=0;j<size;j++)
        {
            std::cout<<C[i*size+j]<<"\t";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";

    Bs.print_report();
}