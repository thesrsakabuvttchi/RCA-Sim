#include"../src/BrainDeadScheduler.h"
int main()
{
    BrainDeadScheduler Bds(2);
    int l,m,n;
    l = 3;
    m = 4;
    n = 3;

    int* A = new int[l * m]();
    int* B = new int[m * n]();
    int* C = new int[l * n]();

    for (int i = 0; i < l; i++) 
    {
        for (int j = 0; j < m; j++)
        {
            A[i*m+j] = i*m+j;
        }
    }

    for (int i = 0; i < m * n; ++i) {
        B[i] = i;
    }

    std::cout<<"Input 1\n";
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<m;j++)
        {
            std::cout<<A[i*m+j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";

    std::cout<<"Input 2\n";
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            std::cout<<B[i*n+j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";

    Bds.scheduleBlockedMM(A,B,C,l,m,n);

    std::cout<<"Output\n";
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<n;j++)
        {
            std::cout<<C[i*n+j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
    Bds.print_report();
}