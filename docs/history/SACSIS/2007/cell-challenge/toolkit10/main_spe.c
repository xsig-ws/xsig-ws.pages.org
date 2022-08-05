/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, ToolKit Version 2007-02-09                */
/*----------------------------------------------------------------------*/
#include <spu_intrinsics.h>
#include <spelib.h>
#include <spe_stdio.h>
#include <stdlib.h>
#include <spu_mfcio.h>
#include <float.h>
#include "spe_util.h"
#include "define.h"

#define DEC_INIT_VAL 0xFFFFFFFF
#define TIME_SYNC 100

extern void spe_sort(struct spe_ctrl* sc);
/*----------------------------------------------------------------------*/
volatile static struct spe_ctrl sc  _GALIGN;
volatile static struct argument arg _GALIGN;
volatile static struct spe_sync_block spe_sync[NUMBER_OF_SPES] _GALIGN;
volatile static struct spe_sync_block sync_addr _GALIGN;
unsigned int dec_cnt;

/*----------------------------------------------------------------------*/
void dmaget_s(void* d, unsigned long long addr, unsigned int size){
    mfc_get(d, addr, size, DMA_TAG, 0, 0);
    mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
    mfc_write_tag_update_all();
    mfc_read_tag_status();
}

/*----------------------------------------------------------------------*/
void dmaput_s(void* d, unsigned long long addr, unsigned int size){
    mfc_put(d, addr, size, DMA_TAG, 0, 0);
    mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
    mfc_write_tag_update_all();
    mfc_read_tag_status();
}

/*----------------------------------------------------------------------*/
void spe_time_start(struct spe_ctrl* sc, unsigned long long argv){
    int i;
    
    dmaget_s((void*)&arg, argv, SPE_DMA_ALIGN);
    
    sc->flag = 0;
    while(sc->flag==0){
        dmaget_s((void*)sc, arg.sc_addr, SPE_DMA_ALIGN);
    }
    
    sync_addr.data[0] = (unsigned int)&spe_sync[0];
    dmaput_s((void*)&sync_addr, 
             sc->sync_addr + sc->id * SPE_DMA_ALIGN, SPE_DMA_ALIGN);
    
    if(sc->id == 0){
        spe_sync[0].data[0] = 1;
        
        spu_write_decrementer(DEC_INIT_VAL);
        dec_cnt = spu_read_decrementer();
        
        for(i=1; i<NUMBER_OF_SPES; i++){
            dmaget_s((void*)&sync_addr, 
                     sc->sync_addr + i * SPE_DMA_ALIGN, SPE_DMA_ALIGN);
            sync_addr.data[1] = sync_addr.data[0] + sc->ls_addr[i];
            
            dmaput_s((void*)&spe_sync[0], sync_addr.data[1], 
                     SPE_DMA_ALIGN);
        }
    }
    else{
        while(spe_sync[0].data[0]!=1);
    }
};

/*----------------------------------------------------------------------*/
void spe_time_end(struct spe_ctrl* sc){
    int i;
    int ret = 1;
    
    if(sc->id == 0){
        while(ret){
            ret = 0;
            for(i=1; i<NUMBER_OF_SPES; i++){
                if(spe_sync[i].data[0] != TIME_SYNC) ret = 1;
            }
        }
        sc->dec_cnt = dec_cnt - spu_read_decrementer();
        dmaput_s((void*)sc, arg.sc_addr, SPE_DMA_ALIGN);
    }
    else{
        spe_sync[0].data[0] = TIME_SYNC;
        dmaget_s((void*)&sync_addr, sc->sync_addr, SPE_DMA_ALIGN);
        sync_addr.data[1] = sync_addr.data[0] + 
            sc->ls_addr[0] + sc->id * SPE_DMA_ALIGN;
        dmaput_s((void*)&spe_sync[0], sync_addr.data[1], SPE_DMA_ALIGN);
    }
};

/*----------------------------------------------------------------------*/
/* SPE main program                                                     */
/*----------------------------------------------------------------------*/
int main(int speid, unsigned long long argv)
{
    struct spe_ctrl *ctrl = (struct spe_ctrl *)&sc;

    spe_time_start(ctrl, argv);
    spe_sort(ctrl);
    spe_time_end(ctrl);
    
    return 0;
}
/*----------------------------------------------------------------------*/
