/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, ToolKit Version 2007-02-09                */
/*----------------------------------------------------------------------*/
#define NUMBER_OF_SPES 7
#define INPUT_DATA (50 * 1024 * 1024)       // input data size
#define USER_MEM   (INPUT_DATA * 2)         // user working memory
#define CTRL_MEM   (16 * 1024)              // 16KB user memory
/*----------------------------------------------------------------------*/
#define DMA_TAG 0
#define SPE_STDIO_BUF_SIZE 512
#define SPE_DMA_ALIGN (128)
#define KEYTYPE_NORM 1 // 
#define KEYTYPE_MAXV 2 // 
/*----------------------------------------------------------------------*/
struct argument{
    int id;               //
    unsigned int sc_addr; //
    float padding[30];    // just for padding
};
/*----------------------------------------------------------------------*/
struct spe_ctrl{
    unsigned int flag;        //
    unsigned int id;          //
    unsigned int spe_num;     //
    unsigned int buf_addr;    //
    unsigned int buf_ctrl;    // 
    unsigned int n;           // the number of element N
    unsigned int listnum;     // list size M
    unsigned int keytype;     //
    unsigned int dec_cnt;     //
    unsigned int sync_addr;   //
    unsigned int ls_addr[NUMBER_OF_SPES];    // 7 * 4 = 28 Byte
    unsigned int padding[22-NUMBER_OF_SPES]; // just for padding
};
/*----------------------------------------------------------------------*/
struct spe_sync_block{
    unsigned int data[32];
};
/*----------------------------------------------------------------------*/
