#include<iostream>
#include<unistd.h>

#ifndef BITMASK
#define BITMASK 0b11111111111111111111111111111100
#endif

class Punit
{
    public:
        int weight,weight_buffer,weight_buffer_next;
        int MulInput,AddInput,MAC_output,delay_cycles,delay_ctr,MulInputNext;
        Punit(int delay)
        {
            this->weight = 0;
            this->weight_buffer = 0;
            this->MulInput = 0;
            this->MulInputNext = 0;
            this->MAC_output = 0;
            this->AddInput = 0;
            this->delay_ctr = 0;
            this->delay_cycles = delay;
        }
        void setMACInputs(int MulInp,int AddInp)
        {
            MulInput = MulInp;
            AddInput = AddInp;
        }
        void setMulInput(int MulInp)
        {
            MulInputNext = this->MulInput;
            MulInput = MulInp;
        }
        void setAddInput(int AddInp)
        {
            AddInput = AddInp;
        }
        void MAC()
        {
            delay_ctr = delay_cycles;
            MAC_output = (weight&BITMASK)*(MulInput&BITMASK)+AddInput;
        }
        void updateWeights(int new_weight)
        {
            weight_buffer_next = weight_buffer;
            weight_buffer = new_weight;
        }
        void switchWeights()
        {
            weight = weight_buffer;
        }
        int cycle(bool start_MAC=false,bool weight_update=false,bool switch_weight=false,int new_weight=0)
        {
            //IF THERE IS A MAC IN PROGRESS (DELAY>0) AND NEW MAC, THROW ERROR
            if(delay_ctr && start_MAC)
            {
                throw std::invalid_argument("Cannot perform a new MAC when previous is running.");
                return -1;
            }
            //MAKE SURE THAT SWITCH WEIGHTS CAN'T HAPPEN ALONG WITH OTHER 2 INSTRUCTIONS
            if(switch_weight && (start_MAC||weight_update))
            {
                throw std::invalid_argument("Cannot switch weights while running a new MAC/weight update");
                return -1;            }

            if(start_MAC)
            {
                MAC();
            }
            if(weight_update)
            {
                updateWeights(new_weight);
            }
            if(switch_weight)
            {
                switchWeights();
            }
            delay_ctr = delay_ctr-1>=0?delay_ctr-1:0;
            return(delay_ctr);
        }
};

// int main()
// {
//     Punit P(3);
//     P.cycle(false,true,false,54);
//     std::cout<<P.weight_buffer_next<<"\n";
//     P.cycle(false,false,true);
//     P.setMACInputs(23,128);
//     P.cycle(true,false,false);
//     std::cout<<"Stalled\n";
//     while(P.cycle())
//     {
//         std::cout<<"Stalled\n";
//     }
//     std::cout<<P.MAC_output;
// }