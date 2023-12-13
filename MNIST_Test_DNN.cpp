#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"NNUtils.h"
#define DENSE1_RNUM 784
#define DENSE1_COLNUM 100
#define DENSE2_RNUM 100
#define DENSE2_COLNUM 10

void readLayerWeights(FILE *fp,int *weights, int *biases, int rownum, int colnum)
{
    char buffer[1024];
    fscanf(fp,"%s",buffer);
    if(strcmp(buffer,"Weights:"))
    {
        printf("Format Error(Weights): please check weight file format\n",buffer);
        exit(-1);
    }
    for(int i=0;i<rownum;i++)
    {
        for(int j=0;j<colnum;j++)
        {
            fscanf(fp,"%d ",&weights[i*colnum+j]);
        }
    }
    printf("Weights read successfully!!\n");

    fscanf(fp,"%s",buffer);
    if(strcmp(buffer,"Biases:"))
    {
        printf("Format Error(Biases): please check weight file format\n",buffer);
        exit(-1);
    }
    for(int j=0;j<colnum;j++)
    {
        fscanf(fp,"%d ",&biases[j]);
    }
    printf("biases read successfully!!\n");
}

void applyRELU(int *results,int size)
{
    for(int i=0;i<size;i++) 
        results[i] = results[i]>0?results[i]:0;
}

int main()
{
    FILE * fp;
    fp = fopen ("weight_file.txt", "r");
    char buffer[1024];

    int *dense1_weights = (int*)malloc(DENSE1_RNUM*DENSE1_COLNUM*sizeof(int));
    int *dense1_biases = (int*)malloc(DENSE1_COLNUM*sizeof(int));

    int *dense2_weights = (int*)malloc(DENSE2_RNUM*DENSE2_COLNUM*sizeof(int));
    int *dense2_biases = (int*)malloc(DENSE2_COLNUM*sizeof(int));

    readLayerWeights(fp,dense1_weights,dense1_biases,DENSE1_RNUM,DENSE1_COLNUM);
    readLayerWeights(fp,dense2_weights,dense2_biases,DENSE2_RNUM,DENSE2_COLNUM);

    fclose(fp);

    fp = fopen ("input_file_MLP.txt", "r");
    int *input = (int*)malloc(DENSE1_RNUM*sizeof(int)),label,predicted;
    int *intermediate = (int*)malloc(DENSE1_COLNUM*sizeof(int));
    int *results = (int*)malloc(DENSE2_COLNUM*sizeof(int));
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
            printf("Format Error(Input): please check input file format %s\n",buffer);
            exit(-1);
        }
        for(int j=0;j<DENSE1_RNUM;j++)
        {
            fscanf(fp,"%d ",&input[j]);
        }

        applyForwardPass(&Bds,input,dense1_weights,dense1_biases,intermediate,DENSE1_RNUM,DENSE1_COLNUM);
        applyRELU(intermediate,DENSE1_COLNUM);
        applyForwardPass(&Bds,intermediate,dense2_weights,dense2_biases,results,DENSE2_RNUM,DENSE2_COLNUM);

        if(!fscanf(fp,"Label:%d",&label))
        {
            printf("Format Error(Label): please check input file format %d\n",label);
            exit(-1);
        }

        total++;
        predicted = getMaxIdx(results,DENSE2_COLNUM);
        if(predicted==label)
            correct++;
        if(total%100==0)
        {
            printf("\n%d Inputs processed.........\nAccuracy Now:\t%f%%\n",total,(float)correct*100/total);
        }
    }
    printf("\n%d Input vectors processed\nAccuracy:%f%%\n",total,(float)correct*100/total);
    Bds.print_report();

    free(dense1_biases);
    free(dense1_weights);
    free(dense2_biases);
    free(dense2_weights);
    free(input);
    free(results);
}
