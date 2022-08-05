/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
/*----------------------------------------------------------------------*/
#define NUMBER_OF_SPES 7
#define EXE_SPE1 "speed/spe1"
#define EXE_SPE2 "speed/spe1"
#define EXE_SPE3 "speed/spe1"
#define EXE_SPE4 "speed/spe1"
#define EXE_SPE5 "speed/spe1"
#define EXE_SPE6 "speed/spe1"
#define EXE_SPE7 "speed/spe1"

#define KEY_TYPE 1 // choose 1 or 2
#define LIST_NUM 7 // list size M
#define INPUT_DATA (30 * 1024 * 1024) // input data size

#define ELEMENT_SIZE (4*(KEY_TYPE + LIST_NUM)) // element size
#define DATA_NUM (INPUT_DATA/ELEMENT_SIZE)     // the number of element N
#define USER_MEM (INPUT_DATA*3)                // user working memory

/*----------------------------------------------------------------------*/
#define DMA_TAG 0
#define SPE_STDIO_BUF_SIZE 512
#define SPE_DMA_ALIGN (128)

/*----------------------------------------------------------------------*/
struct argument{
    int id;               //
    unsigned int sc_addr; //
    float padding[30];    // just for padding
};

struct spe_ctrl{
    unsigned int flag;        //
    unsigned int id;          //
    unsigned int spe_num;     //
    unsigned int buf_addr;    //
    unsigned int n;           // the number of element N
    unsigned int listnum;     // list size M
    unsigned int keytype;     //
    unsigned int padding[25]; // just for padding
};
/*----------------------------------------------------------------------*/
