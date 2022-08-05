/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08         */

#include <stdio.h>
#include <spe/sys/spelib_types.h>
#include <spe_stdio_srv.h>
#include "spe_wrap.h"
#include "ppe_util.h"
#include "define.h"
/*----------------------------------------------------------------------*/
double my_clock();
int generate_list(float*, float*);
void check_list(float*, float*, int);

/*----------------------------------------------------------------------*/
float buf_in[N]  _ALIGN;
float buf_out[N] _ALIGN;

/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int i,n;
    double time_start, time_end;
    swt_handle *h[NUMBER_OF_SPES];
    struct argument spe_arg[NUMBER_OF_SPES];
    _spe_stdio_init(SPE_STDIO_BUF_SIZE);
    _spe_stdio_start();

    n = generate_list(buf_in, buf_out);
    
    for(i=0; i<NUMBER_OF_SPES; i++){
        spe_arg[i].id = i;
        spe_arg[i].n  = n;
        spe_arg[i].adr_in  = (unsigned long long)&buf_in;
        spe_arg[i].adr_out = (unsigned long long)&buf_out;
        h[i] = spe_wrap_create_dedicated_thread(EXE_SPE1, 
                                                (void*)&spe_arg[i],
                                                SIGNOTIFY_INIT_MODE);
    }
    
    time_start = my_clock();
    for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);
    for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
    time_end = my_clock();
    
    for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_release_thread(h[i]);

    check_list(buf_in, buf_out, N);
    printf("[ppe_] elapsed time(sec): %10.6f\n", time_end-time_start);

    return 0;
}
/*----------------------------------------------------------------------*/
