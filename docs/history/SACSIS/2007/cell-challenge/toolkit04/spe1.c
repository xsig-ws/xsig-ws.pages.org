/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
/*----------------------------------------------------------------------*/
#include <spu_intrinsics.h>
#include <spelib.h>
#include <spe_stdio.h>
#include <stdlib.h>
#include <spu_mfcio.h>
#include "spe_util.h"
#include "define.h"
#include <float.h>

/*----------------------------------------------------------------------*/
struct data_block{ // 32 Byte
    float key;
    float data[LIST_NUM];
};

struct transfer_block{ // 128 Byte
    struct data_block dblock[4];
};

struct sync_block{
    unsigned int status[32];
};

struct merge_return{
    int oa;
    int ob;
};
/*----------------------------------------------------------------------*/

volatile static struct argument arg _GALIGN;
volatile static struct spe_ctrl sc _GALIGN;
volatile static struct sync_block sb _GALIGN;
volatile static struct transfer_block eb _GALIGN;

/*----------------------------------------------------------------------*/
void dmaget(void* d, unsigned long long addr, unsigned int size){
    /***** DMA Data Transfer, Memory -> LS *****/
    mfc_get(d, addr, size, DMA_TAG, 0, 0);
    mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
    mfc_write_tag_update_all();
    mfc_read_tag_status();
}

/*----------------------------------------------------------------------*/
void dmaput(void* d, unsigned long long addr, unsigned int size){
    /***** DMA Data Transfer, LS -> Memory *****/
    mfc_put(d, addr, size, DMA_TAG, 0, 0);
    mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
    mfc_write_tag_update_all();
    mfc_read_tag_status();
}
/*----------------------------------------------------------------------*/
float square_norm(float* n, int listnum){
    int i;
    float norm = 0.0;
    for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
    return norm;
};

/*----------------------------------------------------------------------*/
unsigned int get_eaddr(unsigned int saddr, int eid){
    unsigned int eaddr;
    eaddr = saddr + eid * 128;
    return eaddr;
};

/*----------------------------------------------------------------------*/
struct transfer_block get_tblock(int eid, struct spe_ctrl* sc){
    unsigned int eaddr; // element address

    eaddr = get_eaddr(sc->buf_addr,eid);
    dmaget((void*)&eb, eaddr, sizeof(struct transfer_block));
    
    return eb;
};

/*----------------------------------------------------------------------*/
void put_tblock(int eid, struct transfer_block* eblock, struct spe_ctrl* sc){
    unsigned int eaddr; // element address
    eaddr = get_eaddr(sc->buf_addr,eid);
    dmaput((void*)eblock, eaddr, sizeof(struct transfer_block));
};

/*----------------------------------------------------------------------*/
struct data_block get_data(int id, struct spe_ctrl* sc){
    unsigned int eid;
    unsigned int eoffset;
    struct transfer_block eblock;
    
    eid     = id / 4;
    eoffset = id % 4;
    eblock = get_tblock(eid, sc);
    
    return eblock.dblock[eoffset];
};

/*----------------------------------------------------------------------*/
void swap_db(int id1, int id2, struct spe_ctrl* sc){
    struct transfer_block eblock[2];
    unsigned int eid[2];
    unsigned int eoffset[2];
    struct data_block tmp_db;

    eid[0]     = id1 / 4;
    eoffset[0] = id1 % 4;

    eid[1]     = id2 / 4;
    eoffset[1] = id2 % 4;
    
    if(eid[0] == eid[1]){
        eblock[0] = get_tblock(eid[0], sc);
        tmp_db                       = eblock[0].dblock[eoffset[0]];
        eblock[0].dblock[eoffset[0]] = eblock[0].dblock[eoffset[1]];
        eblock[0].dblock[eoffset[1]] = tmp_db;
        put_tblock(eid[0], &eblock[0], sc);
    }
    else{
        eblock[0] = get_tblock(eid[0], sc);
        eblock[1] = get_tblock(eid[1], sc);
        
        tmp_db                       = eblock[0].dblock[eoffset[0]];
        eblock[0].dblock[eoffset[0]] = eblock[1].dblock[eoffset[1]];
        eblock[1].dblock[eoffset[1]] = tmp_db;
        
        put_tblock(eid[0], &eblock[0], sc);
        put_tblock(eid[1], &eblock[1], sc);
    }
};

/*----------------------------------------------------------------------*/
void qs_sort(int s, int e, struct spe_ctrl* sc){
    int l,r;
    struct data_block p;
    struct data_block fl,fr;
    
    p = get_data(((s+e)/2),sc);
    l = s;
    r = e;

    fl = get_data(l,sc);
    fr = get_data(r,sc);

    while(1){
        while(fl.key < p.key){
            l++;
            fl = get_data(l,sc);
        }
        while(fr.key > p.key){
            r--;
            fr = get_data(r,sc);
        }

        if(l >= r)
            break;

        swap_db(l, r, sc);

        l++;
        r--;
        fl = get_data(l,sc);
        fr = get_data(r,sc);
    }

    if(l - s > 1) qs_sort(s, l-1, sc);
    if(e - r > 1) qs_sort(l, e, sc);
};

/*----------------------------------------------------------------------*/
void sort_phase1(struct spe_ctrl* sc){
    int i,j;
    int elnum; // element number
    unsigned int eaddr;
    unsigned int s;
    unsigned int e;

    elnum = sc->n / 16;

    // key calculation
    for(i=0;i<elnum;i++){
        eaddr = get_eaddr(sc->buf_addr, elnum * sc->id + i);
        dmaget((void*)&eb, eaddr, sizeof(struct data_block)*4);
        for(j=0;j<4;j++){
            eb.dblock[j].key = square_norm((float*)eb.dblock[j].data,sc->listnum);
        }
        dmaput((void*)&eb, eaddr, sizeof(struct data_block)*4);
    }

    // Sub sorting in a divided elements
    s = sc->id * elnum * 4;
    e = s + (elnum * 2);
    qs_sort(s,e-1,sc);
    
    s = (sc->id * elnum * 4) + (elnum * 2);
    e = s + (elnum * 2);
    qs_sort(s,e-1,sc);
};

/*----------------------------------------------------------------------*/
void merge_core(struct spe_ctrl* sc, struct merge_return* mr,
                int sa, int sanum, int sb, int sbnum, int da, int danum){
    int i,j,oa,ob;
    struct data_block dba,dbb;
    struct transfer_block eblock;
    int d;
    
    d  = da / 4;
    oa = mr->oa;
    ob = mr->ob;
    
    i = 0;
    j = 0;
    while(i != danum / 4){
        if(oa == sanum){
            eblock.dblock[j] = get_data(sb + ob, sc);
            ob++;
        }
        else if(ob == sbnum){
            eblock.dblock[j] = get_data(sa + oa, sc);
            oa++;
        }
        else{
            dba = get_data(sa + oa, sc);
            dbb = get_data(sb + ob, sc);
            if(dba.key > dbb.key){
                eblock.dblock[j] = dbb;
                ob++;
            }
            else{
                eblock.dblock[j] = dba;
                oa++;
            }
        }

        j++;
        if(j == 4){
            put_tblock(d,&eblock,sc);
            d++;
            i++;
            j = 0;
        }
    }

    mr->oa = oa;
    mr->ob = ob;
}

/*----------------------------------------------------------------------*/
void merge_sort_sub(struct spe_ctrl* sc, int sa, int sanum,int sb, 
                    int sbnum,int da, int danum,int db, int dbnum){
    struct merge_return mr;
    
    mr.oa = 0;
    mr.ob = 0;
    
    merge_core(sc, &mr, sa, sanum, sb, sbnum, da, danum);
    merge_core(sc, &mr, sa, sanum, sb, sbnum, db, dbnum);
};

/*----------------------------------------------------------------------*/
struct sync_block get_status(int id, struct spe_ctrl* sc){
    dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
    return sb;
};

/*----------------------------------------------------------------------*/
void put_status(int id, int sid, int status, struct spe_ctrl* sc){
    dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
    sb.status[sid] = status;
    dmaput((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
};

/*----------------------------------------------------------------------*/
void merge_sort(struct spe_ctrl* sc, int direction, int isa, int isb){
    int elnum; // element number
    int sa,sb,da,db;
    
    elnum = sc->n / 8;
    
    if(direction == 0){
        sa = elnum * isa;
        sb = elnum * isb;
        da = elnum * (isa+8);
        db = elnum * (isb+8);
    }
    else{
        sa = elnum * (isa+8);
        sb = elnum * (isb+8);
        da = elnum * isa;
        db = elnum * isb;
    }
    
    merge_sort_sub(sc, sa, elnum, sb, elnum, da, elnum, db, elnum);
};

/*----------------------------------------------------------------------*/
void wait_spe(struct spe_ctrl* sc, int sid, int status){
    int ret;
    int i;
    int use_spe_num = 4;
    struct sync_block sync;
    
    do{
        ret = 0;
        for(i=0;i<use_spe_num;i++){
            sync = get_status(i,sc);
            if(sync.status[sid] != status){
                ret = 1;
                break;
            }
        }
    }while(ret == 1);
};

/*----------------------------------------------------------------------*/
void sort(struct spe_ctrl* sc){
    int i;
    
    for(i=0; i<2; i++){
        put_status(sc->id, 0, i, sc);
    }
    
    sort_phase1(sc);
    put_status(sc->id, 0, 1, sc);
    wait_spe(sc, 0, 1);
    
    if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
    else if(sc->id == 1) merge_sort(sc, 0, 3, 2);
    else if(sc->id == 2) merge_sort(sc, 0, 4, 5);
    else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
    put_status(sc->id, 1, 2, sc);
    wait_spe(sc, 1, 2);
    
    if(sc->id == 0)      merge_sort(sc, 1, 0, 2);
    else if(sc->id == 1) merge_sort(sc, 1, 1, 3);
    else if(sc->id == 2) merge_sort(sc, 1, 6, 4);
    else if(sc->id == 3) merge_sort(sc, 1, 7, 5);
    put_status(sc->id, 0, 3, sc);
    wait_spe(sc, 0, 3);
    
    if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
    else if(sc->id == 1) merge_sort(sc, 0, 2, 3);
    else if(sc->id == 2) merge_sort(sc, 0, 5, 4);
    else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
    put_status(sc->id, 1, 4, sc);
    wait_spe(sc, 1, 4);
    
    if(sc->id == 0)      merge_sort(sc, 1, 0, 4);
    else if(sc->id == 1) merge_sort(sc, 1, 1, 5);
    else if(sc->id == 2) merge_sort(sc, 1, 2, 6);
    else if(sc->id == 3) merge_sort(sc, 1, 3, 7);
    put_status(sc->id, 0, 5, sc);
    wait_spe(sc, 0, 5);
    
    if(sc->id == 0)      merge_sort(sc, 0, 0, 2);
    else if(sc->id == 1) merge_sort(sc, 0, 1, 3);
    else if(sc->id == 2) merge_sort(sc, 0, 4, 6);
    else if(sc->id == 3) merge_sort(sc, 0, 5, 7);
    put_status(sc->id, 1, 6, sc);
    wait_spe(sc, 1, 6);
    
    if(sc->id == 0)      merge_sort(sc, 1, 0, 1);
    else if(sc->id == 1) merge_sort(sc, 1, 2, 3);
    else if(sc->id == 2) merge_sort(sc, 1, 4, 5);
    else if(sc->id == 3) merge_sort(sc, 1, 6, 7);
    put_status(sc->id, 0, 7, sc);
    wait_spe(sc, 0, 7);
};

/*----------------------------------------------------------------------*/
/* SPE main program                                                     */
/*----------------------------------------------------------------------*/
int main(int speid, unsigned long long argv)
{
    dmaget((void*)&arg, argv, SPE_DMA_ALIGN);
    
    sc.flag = 0;
    
    while(sc.flag==0){
        dmaget((void*)&sc, arg.sc_addr, SPE_DMA_ALIGN);
    }
    
    /*** in this version, only four SPEs (SPE0 to SPE3) work hard! ***/
    if(4>sc.id) sort((struct spe_ctrl*)&sc);
    
    return 0;
}
/*----------------------------------------------------------------------*/
