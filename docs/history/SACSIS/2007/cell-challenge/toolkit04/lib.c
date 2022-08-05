/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
/*----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <spe/sys/spelib_types.h>
#include <spe_stdio_srv.h>

#include "define.h"

struct data_block{ // 32 Byte
    float key;
    float data[LIST_NUM];
};

/*----------------------------------------------------------------------*/
double my_clock()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
};

/*----------------------------------------------------------------------*/

void generate_list(float* buf, int datanum, int listnum){
    int i,j;
    
    srand((unsigned int)time(NULL));
//    srand(1);
    
    for(i = 0; i<datanum; i++){
        buf[(listnum+1)*i] = 0.0;
        for(j = 0; j<listnum; j++){
            buf[(listnum+1)*i+j+1] = (float)rand()/RAND_MAX;
        }
    }
};

/*----------------------------------------------------------------------*/
float square_norm(float* n, int listnum){
    int i;
    float norm = 0.0;
    for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
    return norm;
};

/*----------------------------------------------------------------------*/
void check_list(float* buf,int datanum, int listnum, int keytype){
    int i;
    struct data_block* ssblock;
    
    ssblock = (struct data_block*)buf;
    for(i=1;i<datanum;i++){
        if(ssblock[i - 1].key > ssblock[i].key){
            printf("[ppe_] * Wrong! * n[%d]:k1 %5.10f n[%d] k2 %5.10f\n",
                   i-1, ssblock[i - 1].key, i, ssblock[i].key);
            return;
        }
    }
    printf("[ppe_] * Result Good! *\n");
}
/*----------------------------------------------------------------------*/
