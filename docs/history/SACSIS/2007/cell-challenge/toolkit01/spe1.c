/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08         */

#include <stdlib.h>
#include <spe_stdio.h>
#include <spu_mfcio.h>
#include "spe_util.h"
#include "define.h"
#define SPE_BUF_ENTRY (SPE_BUF_SIZE/sizeof(float))

struct argument arg _GALIGN;
float buf[SPE_BUF_SIZE/sizeof(float)] _GALIGN;

/*----------------------------------------------------------------------*/
void dma_get(char *src, unsigned long long dst, int size)
{
	while(size>0){
	   int dma_size = (size>DMA_MAX_SIZE) ? DMA_MAX_SIZE : size;	
  	   mfc_get(src, dst, dma_size, DMA_TAG, 0, 0);
       mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
       mfc_write_tag_update_all();
       mfc_read_tag_status();
       
       size = size - dma_size;
       src = src + dma_size;
       dst = dst + dma_size;
    } 
}
/*----------------------------------------------------------------------*/
void dma_put(char *src, unsigned long long dst, int size)
{
    while(size>0){
	    int dma_size = (size>DMA_MAX_SIZE) ? DMA_MAX_SIZE : size;
        mfc_put(src, dst, dma_size, DMA_TAG, 0, 0);
        mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
        mfc_write_tag_update_all();
        mfc_read_tag_status(); 
        
        size = size - dma_size;
        src = src + dma_size;
        dst = dst + dma_size;
    } 
}
/*----------------------------------------------------------------------*/
int main(int speid, unsigned long long argv)
{
    dma_get((char *)&arg, argv, sizeof(struct argument));
    spe_printf("[spe%d] let's sort %d items\n", arg.id, arg.n);

    if(arg.n<=SPE_BUF_ENTRY && arg.id==0){ /***** only spe0 works *****/
        dma_get((char *)&buf, arg.adr_in, sizeof(float)*arg.n);
        
        { /***** insertion sort, this is very slow sorting! *****/
            int i,j;
            float temp;
            for(i=1; i<arg.n; i++){
                temp =buf[i]; 
                j=i;
                while(j>0 && buf[j-1]>temp){
                    buf[j] = buf[j-1];
                    j=j-1;
                }
                buf[j] = temp;
            }
        }
        
        dma_put((char *)&buf, arg.adr_out, sizeof(float)*arg.n);
    }
    return 0;
}
/*----------------------------------------------------------------------*/
