#include"Punit.h"
#include<iostream>
class Systolic
{
    public:
        int size,delay;
        Punit *SystolicArray;

    Systolic(int size,int delay)
    {
        this->size = size;
        SystolicArray = (Punit*)malloc(sizeof(Punit)*size*size);
        for(int i=0;i<size*size;i++)
        {
            SystolicArray[i] = Punit(delay);
        }
    }
    Punit *getPunit(int i,int j)
    {
        return(&SystolicArray[i*size+j]);
    }
    void PushWeights(int *weight_inputs)
    {
        for(int i=0;i<size;i++)
        {
            Punit *P_tmp = getPunit(i,0);
            P_tmp->updateWeights(weight_inputs[i]);
        }
        for(int i=0;i<size;i++)
        {
            for(int j=1;j<size;j++)
            {
                Punit *P_tmp = getPunit(i,j);
                Punit *P_tmp_prev = getPunit(i,j-1);
                P_tmp->updateWeights(P_tmp_prev->weight_buffer_next);
            }
        }
    }
    void SwitchWeights()
    {
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                Punit *P_tmp = getPunit(i,j);
                P_tmp->switchWeights();
            }
        }
    }
    int RunMAC(int *multiply_inputs)
    {
        bool stalled = false;
        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                Punit *P_tmp = getPunit(i,j);
                if(P_tmp->delay_ctr>=1)
                {
                    stalled = true;
                    P_tmp->cycle();
                }
            }
        }

        if(stalled)
        {
            return(-1);
        }

        for(int i=0;i<size;i++)
        {
            for(int j=0;j<size;j++)
            {
                Punit *P_tmp = getPunit(i,j);
                stalled = true;
                P_tmp->cycle(true);
            }
        }       

        Punit *P_first = getPunit(0,0);
        P_first->setMulInput(multiply_inputs[0]);
        P_first->setAddInput(0);
        for(int j=1;j<size;j++)
        {
            Punit *P_tmp = getPunit(0,j);
            Punit *P_tmp_prev = getPunit(0,j-1);
            P_tmp->setMulInput(multiply_inputs[j]);
            P_tmp->setAddInput(P_tmp_prev->MAC_output);
        }
        for(int i=1;i<size;i++)
        {
            Punit *P_tmp = getPunit(i,0);
            Punit *P_tmp_prev = getPunit(i-1,0);
            P_tmp->setMulInput(P_tmp_prev->MulInputNext);
            P_tmp->setAddInput(0);
        }
        for(int i=1;i<size;i++)
        {
            for(int j=1;j<size;j++)
            {
                Punit *P_tmp = getPunit(i,j);
                Punit *P_tmp_prev_MAC = getPunit(i,j-1);
                Punit *P_tmp_prev_Mul = getPunit(i-1,j);
                P_tmp->setMulInput(P_tmp_prev_Mul->MulInputNext);
                P_tmp->setAddInput(P_tmp_prev_MAC->MAC_output);
            }
        }
        return(0);
    }
    void cycle()
    {
        //If Weight Update is true, switch the wight with buffered value 

        //Check for stalled PUnits, if stalled advance them

        //Execute the Systolic Array Multiplication

        //Eecute transfer of Buffered Weights
    }

    void print_status(bool weights,bool RMAC_ouptut)
    {
        if(weights)
        {            
            for(int i=0;i<size;i++)
                {
                    for(int j=0;j<size;j++)
                    {
                        Punit *P_tmp = getPunit(i,j);
                        std::cout<<P_tmp->weight<<"\t";
                    }
                    std::cout<<"\n";
                }
                std::cout<<"\n\n";
        }
        else if(RMAC_ouptut)
        {
            for(int i=0;i<size;i++)
            {
                for(int j=0;j<size;j++)
                {
                    Punit *P_tmp = getPunit(i,j);
                    std::cout<<P_tmp->MAC_output<<"\t";
                }
                std::cout<<"\n";
            }
            std::cout<<"\n\n";
        }
    }
};

// int main()
// {
//     Systolic S(3,2);

//     int weigt_new[3] = {0,0,3};
//     S.print_status(true,false);
//     S.PushWeights(weigt_new);

//     weigt_new[1] = 3;
//     weigt_new[2] = 0;
//     S.print_status(true,false);
//     S.PushWeights(weigt_new);

//     weigt_new[0] = 3;
//     weigt_new[1] = 0;
//     S.print_status(true,false);
//     S.PushWeights(weigt_new);

//     S.SwitchWeights();
//     S.print_status(true,false);

//     int multiply_inputs[3] = {4,5,6};
//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);

//     S.RunMAC(multiply_inputs);
//     S.print_status(false,true);
// }