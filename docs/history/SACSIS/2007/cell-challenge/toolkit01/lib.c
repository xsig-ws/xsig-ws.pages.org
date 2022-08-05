/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08         */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <spe/sys/spelib_types.h>
#include <spe_stdio_srv.h>

#include "define.h"
/*----------------------------------------------------------------------*/
double my_clock()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
};

/*----------------------------------------------------------------------*/
int generate_list(float* buf_in, float* buf_out)
{
    int i;
    srand(0);
    for(i=0; i<N; i++){
        buf_in[i]  = (1000.0*rand()/(RAND_MAX+1.0));
        buf_out[i] = 0.0;
    }
    return N;
}

/*----------------------------------------------------------------------*/
void check_list(float* buf_in, float* buf_out, int n)
{
    int i, error=0;
    double sum_in=0.0, sum_out=0.0;
    printf("[ppe_] print the first ten items.\n");
    for(i=0; i<10 && i<n; i++){
        printf("[ppe_] item %3d: %9.3f %9.3f\n", i, buf_in[i], buf_out[i]);
    }
    for(i=0; i<n; i++){
        sum_in  += buf_in[i];
        sum_out += buf_out[i];
        if((i+i)<n && buf_out[i+1]<buf_out[i]) error=1;
    }
    if(sum_in==sum_out && error==0) printf("[ppe_] ** Result: Good! **\n");
    else printf("[ppe_] ** Result: Wrong! **\n");
}
/*----------------------------------------------------------------------*/
