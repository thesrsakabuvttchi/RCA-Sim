#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"NNUtils.h"
#define ROWNUM 784
#define COLNUM 10

int main()
{
    FILE * fp;
    fp = fopen ("weight_file_MLP.txt", "r");
    char buffer[1024];

    int *weights = (int*)malloc(ROWNUM*COLNUM*sizeof(int));
    int *biases = (int*)malloc(COLNUM*sizeof(int));

    fscanf(fp,"%s",buffer);
    if(strcmp(buffer,"Weights:"))
    {
        printf("Format Error(Weights): please check weight file format\n",buffer);
        exit(-1);
    }
    for(int i=0;i<ROWNUM;i++)
    {
        for(int j=0;j<COLNUM;j++)
        {
            fscanf(fp,"%d ",&weights[i*COLNUM+j]);
        }
    }
    printf("Weights read successfully!!\n");

    fscanf(fp,"%s",buffer);
    if(strcmp(buffer,"Biases:"))
    {
        printf("Format Error(Biases): please check weight file format\n",buffer);
        exit(-1);
    }
    for(int j=0;j<COLNUM;j++)
    {
        fscanf(fp,"%d ",&biases[j]);
    }
    printf("biases read successfully!!\n");

    fclose(fp);

    fp = fopen ("input_file_MLP.txt", "r");
    int *input = (int*)malloc(ROWNUM*sizeof(int)),label,predicted;
    int *results = (int*)malloc(COLNUM*sizeof(int));
    BrainDeadScheduler Bds(8);
    int total=0,correct=0;


    while(1)
    {
        fscanf(fp,"%s",buffer);
        if(strcmp(buffer,"END")==0)
        {
            break;
        }
        if(strcmp(buffer,"Input:"))
        {
            printf("Format Error(Input): please check input file format\n",buffer);
            exit(-1);
        }
        for(int j=0;j<ROWNUM;j++)
        {
            fscanf(fp,"%d ",&input[j]);
        }

        applyForwardPass(&Bds,input,weights,biases,results,ROWNUM,COLNUM);
        
        if(!fscanf(fp,"Label:%d",&label))
        {
            printf("Format Error(Label): please check input file format %d\n",label);
            exit(-1);
        }

        total++;
        predicted = getMaxIdx(results,COLNUM);
        if(predicted==label)
            correct++;
        if(total%100==0&&total>1)
        {
            printf("\n%d Inputs processed.........\nAccuracy Now:\t%f%%\n",total,(float)correct*100/total);
        }
    }
    printf("\n%d Input vectors processed\nAccuracy:%f%%\n",total,(float)correct*100/total);
    Bds.print_report();

    free(weights);
    free(biases);
    free(input);
    free(results);
}
 