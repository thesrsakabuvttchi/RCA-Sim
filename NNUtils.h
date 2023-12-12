#include"BrainDeadScheduler.h"
int getMaxIdx(int *arr,int size)
{
    int max_idx=0;
    for(int i=0;i<size;i++)
    {
        if(arr[max_idx]<arr[i])
            max_idx=i;
    }
    return(max_idx);
}

void applyForwardPass(BrainDeadScheduler *Bds,int *input, int *weights, 
                        int *biases,int *results,int inputSize, int outputSize)
{
    Bds->scheduleBlockedMM(input,weights,results,1,inputSize,outputSize);
    for(int i=0;i<outputSize;i++)
    {
        results[i]+=biases[i];
    }
}