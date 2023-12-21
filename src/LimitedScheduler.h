#include"Systolic.h"
#include<iostream>

class BaseScheduler{
    public:
    Systolic S;
    int size;
    int weight_load_stalls,multiplication_cycles,wasted_input_cycles,weight_switches;

    BaseScheduler(int size=4,int delay=1)
    {
        this->S = Systolic(size,delay);
        this->size = size;
        this->weight_load_stalls = 0;
        this->multiplication_cycles = 0;
        this->wasted_input_cycles = 0;
        this->weight_switches = 0;
    }

    void transformWeights(int *weights,int *out)
    {
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                out[i*size+j] = weights[j*size+(size-i-1)];
            }
        }
    }

    void transformMultiplicands(int *Mul,int *BPadded)
    {
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                BPadded[(i+j)*size+j] = Mul[j*size+i];
            }
        }   
    }

    void scheduleMM(int *A,int *B, int *C)
    {
        int weights[size],ATransform[size*size];
        // do the transform by a function
        transformWeights(A,ATransform);
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                weights[j] = ATransform[i*size+j];
            }
            S.PushWeights(weights);
            this->weight_load_stalls++;
        }

        S.SwitchWeights();
        this->weight_switches++;

        //Need to pad out the input matrix now
        int *BPadded = new int [size * (3*size-1)]();
        int *output = new int [size];
        transformMultiplicands(B,BPadded);

        int multiplicands[size];
        for(int i=0;i<3*size-1;i++)
        {
            for(int j=0;j<size;j++)
            {
                multiplicands[j] = BPadded[i*size+j];
            }
            this->multiplication_cycles+=S.RunMAC(multiplicands);
            S.returnOutput(output);

            for(int k=0;k<size;k++)
            {
                if(i-size-k>=0 && i-size-k<size) // Ensure that the padded ouput falls within the bounds of output array
                {
                    C[((i-size-k)+k*size)] = output[k];
                }
            }
        }   
        this->wasted_input_cycles+=size;
        delete[] BPadded;
        delete[] output;
    }

    void print_report()
    {
        std::cout<<"Reporting Stats RCA Sim V0.12 - Limited Scheduler\n";
        std::cout<<"________________________________________________________________\n";
        std::cout<<"Stalls due to weight loads:\t"<<weight_load_stalls;
        std::cout<<"\nNumber of weight Switches:\t"<<weight_switches;
        std::cout<<"\nCycles where multipliers were active:\t"<<multiplication_cycles;
        std::cout<<"\nCycles with wasted inputs (all 0):\t"<<wasted_input_cycles;
        std::cout<<"\nTotal Cycles for scheduling policy:\t"<<weight_load_stalls+weight_switches+multiplication_cycles;
        std::cout<<"\n__________________________________________________________________\n";
    }
};
