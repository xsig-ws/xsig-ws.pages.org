/*----------------------------------------------------------------------*/
/* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08         */
/*----------------------------------------------------------------------*/
#define N 1024*32   /* the number of items, please modify this number */

#define NUMBER_OF_SPES 1
#define EXE_SPE1 "speed/spe1"
#define EXE_SPE2 "speed/spe1"
#define EXE_SPE3 "speed/spe1"
#define EXE_SPE4 "speed/spe1"
#define EXE_SPE5 "speed/spe1"
#define EXE_SPE6 "speed/spe1"
#define EXE_SPE7 "speed/spe1"

#define SPE_BUF_SIZE (128*1024) /* allocate a 128KB buffer */

/*----------------------------------------------------------------------*/
#define DMA_TAG 0
#define SPE_STDIO_BUF_SIZE 256
#define DMA_MAX_SIZE 16384
/*----------------------------------------------------------------------*/
struct argument
{
    int id;
    int n;
    float *buf_in;
    float *buf_out;
    long long adr_in;
    long long adr_out;
};
/*----------------------------------------------------------------------*/
