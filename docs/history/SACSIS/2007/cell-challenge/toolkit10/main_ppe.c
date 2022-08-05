/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, ToolKit Version 2007-02-09                */
/*----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fenv.h>
#include <spe/sys/spelib_types.h>
#include <spe_stdio_srv.h>
#include <spere_cbea.h>
#include "spe_wrap.h"
#include "ppe_util.h"
#include "define.h"
#include "spe.h"

/*----------------------------------------------------------------------*/
double my_clock();
void generate_list(float*, int, int*, int*, int*);
void check_list(float*, int, int, int, int);

volatile static char  buf_ctrl[CTRL_MEM]   _ALIGN;
volatile static float buf_data[USER_MEM/4] _ALIGN;

volatile static struct argument sarg[NUMBER_OF_SPES] _ALIGN;
volatile static struct spe_ctrl ctrl[NUMBER_OF_SPES] _ALIGN;
volatile static struct spe_sync_block sync_addr[NUMBER_OF_SPES] _ALIGN;
double t1,t2; // start and end time stamp

/*----------------------------------------------------------------------*/
void ppe_sort(volatile float* buf_data, int n, int listnum, int keytype){
    int i,j;
    swt_handle *h[NUMBER_OF_SPES];
    spere_object_id hint[NUMBER_OF_SPES][5];
    spere_addr ls_addr[NUMBER_OF_SPES];
    char *spe_obj[7] = {EXE_SPE1, EXE_SPE2, EXE_SPE3, EXE_SPE4, 
                        EXE_SPE5, EXE_SPE6, EXE_SPE7};
    
    _spe_stdio_init(SPE_STDIO_BUF_SIZE);
    _spe_stdio_start();

    for(i=0; i<NUMBER_OF_SPES; i++){
        h[i] = spe_wrap_create_dedicated_thread(
            spe_obj[i], (void*)&sarg[i], SIGNOTIFY_INIT_MODE);
        sarg[i].id      = i;
        sarg[i].sc_addr = (unsigned int)&ctrl[i];
        spere_spe_thread_map_ls(h[i]->id, &ls_addr[i]);
    }
    
    for(i=0; i<NUMBER_OF_SPES; i++){
        ctrl[i].flag     = 0;
        ctrl[i].id       = i;
        ctrl[i].buf_addr = (unsigned int)buf_data;

        ctrl[i].buf_ctrl = (unsigned int)buf_ctrl;

        ctrl[i].n        = n;
        ctrl[i].listnum  = listnum;
        ctrl[i].spe_num  = NUMBER_OF_SPES;
        ctrl[i].keytype  = keytype;
        ctrl[i].dec_cnt  = 0;
        ctrl[i].sync_addr = (unsigned int)sync_addr;
        for(j=0;j<NUMBER_OF_SPES;j++){
            ctrl[i].ls_addr[j] = ls_addr[j];
       }
    }
    
    for(i=0; i<NUMBER_OF_SPES; i++){
        hint[i][0] = spe_wrap_add_access_hint(
            h[i]->id, (void*)&sarg[i], SPE_DMA_ALIGN);
        hint[i][1] = spe_wrap_add_access_hint(
            h[i]->id, (void*)&ctrl[i], SPE_DMA_ALIGN);
        hint[i][2] = spe_wrap_add_access_hint(
            h[i]->id, (void*)buf_data, USER_MEM);

        hint[i][3] = spe_wrap_add_access_hint(
            h[i]->id, (void*)buf_ctrl, 16*1024);

        hint[i][4] = spe_wrap_add_access_hint(
            h[i]->id, (void*)&sync_addr[0], 
            sizeof(struct spe_sync_block)*NUMBER_OF_SPES);
    }
    
    for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);
    
    /**** start the sorting *****/
    t1 = my_clock();
    for(i=0; i<NUMBER_OF_SPES; i++) ctrl[i].flag = 1;
    for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
    t2 = my_clock();
    /**** end   the sorting *****/
    
    for(i=0; i<NUMBER_OF_SPES; i++){
        spe_wrap_remove_access_hint(h[i]->id, hint[i][0]);
        spe_wrap_remove_access_hint(h[i]->id, hint[i][1]);
        spe_wrap_remove_access_hint(h[i]->id, hint[i][2]);
        spe_wrap_remove_access_hint(h[i]->id, hint[i][3]);

        spe_wrap_remove_access_hint(h[i]->id, hint[i][4]);

        spere_spe_thread_unmap_ls(h[i]->id);
        spe_wrap_release_thread(h[i]);
    }
    _spe_stdio_end();
};

/*----------------------------------------------------------------------*/
/* PPE main program                                                     */
/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int i;
    int problem=1; /* problem identification, from 1 to 10  */
    int n;         /* the number of elements                */
    int m;         /* list length                           */
    int keytype;   /* keytype, KEYTYPE_NORM or KEYTYPE_MAXV */
    float *bufp = (float *)buf_data;
    
    if(argc >= 2){
        problem = atoi(argv[1]);
        if(problem<0 || problem>10) problem=1;
    }
    
    generate_list(bufp, problem, &n, &m, &keytype);
    for(i=0; i<CTRL_MEM; i++) buf_ctrl[i]=0;
    
    ppe_sort(bufp, n, m, keytype);
    
    check_list(bufp, problem, n, m, keytype);
    
    printf("[ppe_] problem=%d\n", problem);
    printf("[ppe_] n=%d, m=%d, input_data=%d [MByte]\n",
           n, m, sizeof(float)*n*(m+1)/1024/1024);
    printf("[ppe_] elapsed time by SPE(sec) %8.6f\n", 
           ctrl[0].dec_cnt / (100.0 * 1000.0 * 1000.0));
    printf("[ppe_] elapsed time by PPE(sec) %8.6f\n", t2-t1);
    
    return 0;
}
/*----------------------------------------------------------------------*/
