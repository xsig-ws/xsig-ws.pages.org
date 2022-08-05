/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
/*----------------------------------------------------------------------*/
#include <stdio.h>
#include <spe/sys/spelib_types.h>
#include <spe_stdio_srv.h>
#include "spe_wrap.h"
#include "ppe_util.h"
#include "define.h"
#include <float.h>
#include <fenv.h>
#include <spere_cbea.h>

/*----------------------------------------------------------------------*/
double my_clock();
void generate_list(float*, int, int);
void check_list(float*, int, int, int);

volatile static float buf[USER_MEM/4] _ALIGN;
volatile static struct argument sarg[NUMBER_OF_SPES] _ALIGN;
volatile static struct spe_ctrl ctrl[NUMBER_OF_SPES] _ALIGN;
double t1,t2; // start and end time stamp

/*----------------------------------------------------------------------*/
void ppe_sort(volatile float* buf, int n, int listnum, int keytype){
   int i;
   swt_handle *h[NUMBER_OF_SPES];
   spere_object_id hint[NUMBER_OF_SPES][3];

   _spe_stdio_init(SPE_STDIO_BUF_SIZE);
   _spe_stdio_start();

   for(i=0; i<NUMBER_OF_SPES; i++){
       h[i] = spe_wrap_create_dedicated_thread(
           EXE_SPE1, (void*)&sarg[i], SIGNOTIFY_INIT_MODE);
       sarg[i].id      = i;
       sarg[i].sc_addr = (unsigned int)&ctrl[i];
   }

   for(i=0; i<NUMBER_OF_SPES; i++){
       ctrl[i].flag     = 0;
       ctrl[i].id       = i;
       ctrl[i].buf_addr = (unsigned int)buf;
       ctrl[i].n        = n;
       ctrl[i].listnum  = listnum;
       ctrl[i].spe_num  = NUMBER_OF_SPES;
   }

   for(i=0; i<NUMBER_OF_SPES; i++){
       hint[i][0] = spe_wrap_add_access_hint(
           h[i]->id, (void*)&sarg[i], SPE_DMA_ALIGN);
       hint[i][1] = spe_wrap_add_access_hint(
           h[i]->id, (void*)&ctrl[i], SPE_DMA_ALIGN);
       hint[i][2] = spe_wrap_add_access_hint(
           h[i]->id, (void*)buf, USER_MEM);
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
       spe_wrap_release_thread(h[i]);
   }
   _spe_stdio_end();
/*
   {
       int j;
       for(i=0;i<n;i++){
           printf("%4d : key %3.3f : ",i,buf[i*(listnum+1)]);
           for(j=0;j<listnum;j++){
               printf("%3.3f ",buf[i*(listnum+1)+j+1]);
           }
           printf("\n");
       }
   }
*/
};

/*----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
   int n       = DATA_NUM;
   int listnum = LIST_NUM;
   int keytype = KEY_TYPE;

   if(argc == 4){
       n       = atoi(argv[1]);
       listnum = atoi(argv[2]);
       keytype = atoi(argv[3]);
   }

   generate_list((float*)buf, n, listnum);
   ppe_sort((volatile float*)buf, n, listnum, keytype);
   check_list((float*)buf, n, listnum, keytype);

   printf("[ppe_] n %d, listnum %d, input data size %d [MByte]\n",
          n, listnum, sizeof(float)*n*(listnum+1)/1024/1024);
   printf("[ppe_] elapsed time(sec) %8.6f\n", t2-t1);
   return 0;
}
/*----------------------------------------------------------------------*/
