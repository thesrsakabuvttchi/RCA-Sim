//Later fix the issue when n is an odd number, random zeroes
#include"LimitedScheduler.h"
#include<iostream>

class BrainDeadScheduler : BaseScheduler{
    public:
    BrainDeadScheduler(int size) : BaseScheduler(size)
    {

    }

    void scheduleBlockedMM(int *A, int *B, int *C, int m, int n, int p)
    {
        int m_padded = (m + size - 1) / size * size;
        int n_padded = (n + size - 1) / size * size;
        int p_padded = (p + size - 1) / size * size;

        int* A_padded = new int[m_padded * n_padded]();  // Zero-initialized
        int* B_padded = new int[n_padded * p_padded]();  // Zero-initialized
        int* C_padded = new int[m_padded * p_padded]();  // Zero-initialized

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                A_padded[i * n_padded + j] = A[i * n + j];
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < p; ++j) {
                B_padded[i * p_padded + j] = B[i * p + j];
            }
        }

        // Split the arrays into blocks
        for (int i = 0; i < m_padded; i += size) {
            for (int j = 0; j < p_padded; j += size) {
                for (int k = 0; k < n_padded; k += size) {

                    //Create temporary matrices to hold blocked inputs
                    int *A_tmp = new int [size * size]();
                    int *B_tmp = new int [size * size]();
                    int *C_tmp = new int [size * size]();

                    for (int ii = i; ii < i + size; ++ii) {
                        for (int jj = j; jj < j + size; ++jj) {
                            for (int kk = k; kk < k + size; ++kk) {
                            A_tmp[(ii-i)*size+(kk-k)] = A_padded[ii * n_padded + kk];
                            B_tmp[(kk-k)*size+(jj-j)] = B_padded[kk * p_padded + jj];
                            }
                        }
                    }

                    scheduleMM(A_tmp,B_tmp,C_tmp);
                    for(int ii=0;ii<size;ii++)
                    {
                        for(int jj=0;jj<size;jj++)
                        {
                            C_padded[(i+ii)*p_padded+j+jj] += C_tmp[ii*size+jj]; 
                        }
                    }
                }
            }
        }

        // Copy result from padded matrix to output matrix C
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < p; ++j) {
                C[i * p + j] = C_padded[i * p_padded + j];
            }
        }

        // Deallocate memory
        delete[] A_padded;
        delete[] B_padded;
        delete[] C_padded;
    }

    void print_report()
    {
        std::cout<<"Reporting Stats RCA Sim V0.12 - Brain Dead Scheduler\n";
        std::cout<<"________________________________________________________________\n";
        std::cout<<"Stalls due to weight loads:\t"<<weight_load_stalls;
        std::cout<<"\nNumber of weight Switches:\t"<<weight_switches;
        std::cout<<"\nCycles where multipliers were active:\t"<<multiplication_cycles;
        std::cout<<"\nCycles with wasted inputs (all 0):\t"<<wasted_input_cycles;
        std::cout<<"\nTotal Cycles for scheduling policy:\t"<<weight_load_stalls+weight_switches+multiplication_cycles;
        std::cout<<"\n__________________________________________________________________\n";
    }
};

// int main()
// {
//     BrainDeadScheduler Bds(2);
//     int l,m,n;
//     l = 4;
//     m = 3;
//     n = 2;

//     int* A = new int[l * m]();
//     int* B = new int[m * n]();
//     int* C = new int[l * n]();

//     for (int i = 0; i < l; i++) 
//     {
//         for (int j = 0; j < m; j++)
//         {
//             A[i*m+j] = i*m+j;
//         }
//     }

//     for (int i = 0; i < m * n; ++i) {
//         B[i] = i;
//     }

//     for(int i=0;i<l;i++)
//     {
//         for(int j=0;j<m;j++)
//         {
//             std::cout<<A[i*m+j]<<" ";
//         }
//         std::cout<<"\n";
//     }
//     std::cout<<"\n";

//     for(int i=0;i<m;i++)
//     {
//         for(int j=0;j<n;j++)
//         {
//             std::cout<<B[i*n+j]<<" ";
//         }
//         std::cout<<"\n";
//     }
//     std::cout<<"\n";

//     Bds.scheduleBlockedMM(A,B,C,l,m,n);

//     for(int i=0;i<l;i++)
//     {
//         for(int j=0;j<n;j++)
//         {
//             std::cout<<C[i*n+j]<<" ";
//         }
//         std::cout<<"\n";
//     }

//     Bds.print_report();
// }