<------------------------------------------------------------------------->
Cell Speed Challenge 2007
�������Ńc�[���L�b�g Version 2007-01-08 �̎g�����Ɖ��

�y�K��ۑ�T�v�z
�����̃f�[�^�̌n�񂪓��͂Ƃ��ė^����ꂽ�Ƃ��ɁC���������鏇���ɂ���
�����ĕ��ёւ��鏈�����\�[�e�B���O�ƌĂт܂��DCell�X�s�[�h�`�������W2007
�̋K��ۑ�́u�\�[�e�B���O�v�ł��D
�\�[�e�B���O�ł́C�f�[�^�Ɋ܂܂����בւ��̂��߂ɎQ�Ƃ���鍀�ڂ��L�[��
�Ăт܂��D���̃L�[�͒P���x�̕��������_��(C����ɂ�����float�^)�Ƃ��܂��D
�܂��C�L�[�̒l������(�������l���O���C�傫���l������ɂȂ�悤�ȏ���)��
�Ȃ�悤�Ƀf�[�^����ёւ�����̂Ƃ��܂��D
���Ƃ��΁C���Z�Ȃǂ̊w�Ж����n�̃f�[�^�n��(A1, A2, ..., An)���C
�w�Дԍ�����(�w�Дԍ����L�[�Ƃ���)���ёւ��鏈�����l���܂��D
�e�f�[�^Ai�́C�w�Дԍ��̑��ɁC���̔ԍ������w��������w�N�����Ȃǂ̏��
������������܂���D
�\�[�e�B���O�͂�����Ai���w�Дԍ��̏��������ɕ��ёւ���Ƃ��������ł��D

�y��P�� �������Ńc�[���L�b�g�̎g�����z

Cell�I�����C���T�[�r�X celldev3.celltsb.com �Ƀ��O�C�����Ă��������D
�z�[���f�B���N�g���� speed �Ƃ����f�B���N�g�����쐬���C
�����ɃT���v���t�@�C���R�s�[���܂��D���̂��߂̃R�}���h�͎��̒ʂ�ł��D

$ cd ~
$ mkdir speed
$ cp /export/home/cell2007/share/toolkit02/* .

�R���p�C������ю��s�̂��߂ɂ́C�쐬���� speed �Ƃ����f�B���N�g���Ɉړ�
���āCmake �R�}���h�𗘗p���܂��D���̂��߂̃R�}���h�͎��̒ʂ�ł��D

$ cd ~/speed
$ make
$ make run

�Q�Ԗڂ�make�ŃR���p�C���C�R�Ԗڂ�make run�Ŏ��s���J�n���܂��D
32768�̃f�[�^�̃\�[�e�B���O�������Ȃ��C���̌��ʂ��������ꍇ�ɂ́C
[ppe_] * Result Good! *
�Ƃ��������b�Z�[�W�ƌv�Z�ɗv�������Ԃ��o�͂��܂��D

�y��Q�� �������Ńc�[���L�b�g�̃t�@�C���\���z

�쐬�����f�B���N�g���ɂ͎��̃t�@�C�����R�s�[����܂��D

(1) README.txt  : ���̃t�@�C��
(2) Code.txt    : ���ׂẴ\�[�X�R�[�h���܂Ƃ߂��e�L�X�g�t�@�C��
(3) Makefile    : make �R�}���h�����p����t�@�C��
(4) define.h    : ���ׂẴv���O�������C���N���[�h����t�@�C��
(5) main.c      : PPE�̂��߂̃��C���֐����܂�C�v���O����
(6) lib.c       : PPE�̃v���O���������p����C�̊֐��Q
(7) spe1.c      : SPE�̂��߂̃��C���֐����܂�C�v���O����

�ʏ�́C(1), (2), (3), (6) �̃t�@�C����ύX����K�v�͂���܂���D
(4), (5), (7) �̂R�̃t�@�C����ύX���āC�����ȃ\�[�e�B���O�v���O������
������ڎw���Ă��������D

�y��R�� �������Ńc�[���L�b�g�̃v���O��������z

��������́C�v���O������Makefile�̉���ł��D
�������̃X�y�[�X�Ɛ����i�R�[�h�̍s�ԍ��j����n�܂�s�́C�\�[�X�R�[�h��
�ꕔ�ł��D����ȊO�̍s������ł��D

(4) define.h    : ���ׂẴv���O�������C���N���[�h����t�@�C��
(6) lib.c       : PPE�̃v���O���������p����֐��Q
(5) main.c      : PPE�̂��߂̃��C���֐����܂ރv���O����
(7) spe1.c      : SPE�̂��߂̃��C���֐����܂ރv���O����
(3) Makefile    : make �R�}���h�����p����t�@�C��
���̏����Ńv���O�����̃R�[�h���݂Ă����܂��D

���ӓ_�D
���̃\�[�X�R�[�h�͂������Ńc�[���L�b�g�̂��̂ł��D���̔ł̃c�[���L�b�g��
�v���O���~���O�̓����Ƃ��ė��p�ł���悤�ɃV���v���ł��܂��܂ȍœK���̗]�n
�̂�����̂�񋟂��Ă��܂��D�R���e�X�g�ŗ��p���鐳���ȃc�[���L�b�g�ł́C
���e���啝�ɕύX�����\��������܂��D

file name: define.h
   1 /*-------------------------------------------------------------------*/
   2 /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08      */
   3 /*-------------------------------------------------------------------*/
   4 #define N 1024*32   /* the number of items, please modify this */
�����Ń\�[�e�B���O�̃f�[�^�����w�肵�܂��D�����ł́C32k�̃f�[�^���w�肵��
���܂��DSPE��Local Store�̍�Ɨ̈�Ƃ���128KB��ݒ肵�Ă���̂ŁC�f�[�^��
��32k��葽���ݒ肷��ƃG���[�ƂȂ�ꍇ������܂��D�܂��́C32k��菭�Ȃ�
�͈͂œ���m�F�����Ă��������D
���ۂ̃R���e�X�g�ł͑�K�͂ȃf�[�^���������邱�ƂɂȂ�\��ł��D

   5
   6 #define NUMBER_OF_SPES 1
���p����SPE�̐����w�肵�܂��D�����ł͂P��ݒ肵�Ă��܂��D
�P�`�V�̐�����ݒ肵�܂��D

   7 #define EXE_SPE1 "speed/spe1"
   8 #define EXE_SPE2 "speed/spe1"
   9 #define EXE_SPE3 "speed/spe1"
  10 #define EXE_SPE4 "speed/spe1"
  11 #define EXE_SPE5 "speed/spe1"
  12 #define EXE_SPE6 "speed/spe1"
  13 #define EXE_SPE7 "speed/spe1"
���ꂼ���SPE�����s����v���O�����̃t�@�C�������w�肵�܂��D�t�@�C������
���[�U�̃f�B���N�g������̃p�X���w�肵�܂��D
�����ł́C���ׂĂ�SPE��speed/spe1�Ƃ������O�̃v���O�����𗘗p����ݒ��
�Ȃ��Ă��܂��D���ꂼ���SPE���قȂ�v���O���������s���Ă����܂��܂���D

  14
  15 #define SPE_BUF_SIZE (128*1024) /* allocate a 128KB buffer */
�����Ŏw�肵���T�C�Y�̃o�b�t�@��SPE�ɐ������Ă��܂��D
SPE��Local Store��256KB�Ȃ̂ŁC���̔�����128KB�ɐݒ肵�Ă��܂��D
���̒l�͓K�؂ɒ������Ă��������D

  16
  17 /*-------------------------------------------------------------------*/
  18 #define DMA_TAG 0
  19 #define SPE_STDIO_BUF_SIZE 256
  20 #define DMA_MAX_SIZE 16384
  21 /*-------------------------------------------------------------------*/
  22 struct argument
  23 {
  24     int id;
  25     int n;
  26     float *buf_in;
  27     float *buf_out;
  28     long long adr_in;
  29     long long adr_out;
  30 };
  31 /*-------------------------------------------------------------------*/
���̍\���̂́CPPE����SPE�ɏ���`���邽�߂ɗ��p���܂��D
SPE�ł́C�܂����̍\���̂̃f�[�^����M���ď�����i�߂܂��D����ɂ��C
�\�[�e�B���O�̃f�[�^����C�f�[�^���i�[����Ă��郁�����̃A�h���X��m��
���Ƃ��ł��܂��D

file name: lib.c
   1 /*-------------------------------------------------------------------*/
   2 /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08      */
   3
   4 #include <stdio.h>
   5 #include <stdlib.h>
   6 #include <sys/time.h>
   7 #include <spe/sys/spelib_types.h>
   8 #include <spe_stdio_srv.h>
   9 #include "define.h"
  10 /*-------------------------------------------------------------------*/
  11 double my_clock()
  12 {
  13     struct timeval tv;
  14     gettimeofday(&tv, NULL);
  15     return tv.tv_sec + (double)tv.tv_usec*1e-6;
  16 };
  17
  18 /*-------------------------------------------------------------------*/
  19 int generate_list(float* buf_in, float* buf_out)
  20 {
  21     int i;
  22     srand(0);
  23     for(i=0; i<N; i++){
  24         buf_in[i]  = (1000.0*rand()/(RAND_MAX+1.0));
  25         buf_out[i] = 0.0;
  26     }
  27     return N;
  28 }
  29
  30 /*-------------------------------------------------------------------*/
  31 void check_list(float* buf_in, float* buf_out, int n)
  32 {
  33     int i, error=0;
  34     double sum_in=0.0, sum_out=0.0;
  35     printf("[ppe_] print the first ten items.\n");
  36     for(i=0; i<10 && i<n; i++){
  37         printf("[ppe_] item %3d: %9.3f %9.3f\n",
  38                i, buf_in[i], buf_out[i]);
  39     }
  40     for(i=0; i<n; i++){
  41         sum_in  += buf_in[i];
  42         sum_out += buf_out[i];
  43         if((i+i)<n && buf_out[i+1]<buf_out[i]) error=1;
  44     }
  45     if(sum_in==sum_out && error==0)
  46         printf("[ppe_] * Result Good! *\n");
  47     else printf("[ppe_] * Result Wrong! *\n");
  48 }
  49 /*-------------------------------------------------------------------*/
�ʏ�́C����� lib.c �̓��e��ύX����K�v�͂���܂���D
���̃c�[���L�b�g�ł́C�����ɂ����ёւ��̃L�[�𐶐����Ă��܂����C
�R���e�X�g�ł͗����𗘗p����Ƃ͌���܂���D
�܂��C���̃c�[���L�b�g�ł́C�f�[�^�̓L�[�݂̂���\���������̂Ƃ���
���܂����C���ۂ̃f�[�^�̓L�[�Ƃ��̑��̏�񂩂�\������邱�Ƃɒ��ӂ���
���������D

file name: main.c
   1 /*-------------------------------------------------------------------*/
   2 /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08      */
   3
   4 #include <stdio.h>
   5 #include <spe/sys/spelib_types.h>
   6 #include <spe_stdio_srv.h>
   7 #include "spe_wrap.h"
   8 #include "ppe_util.h"
   9 #include "define.h"
  10 /*-------------------------------------------------------------------*/
  11 double my_clock();
  12 int generate_list(float*, float*);
  13 void check_list(float*, float*, int);
  14
  15 /*-------------------------------------------------------------------*/
  16 float buf_in[N]  _ALIGN;
  17 float buf_out[N] _ALIGN;
main.c �� PPE �����s����v���O�����ł��D
�Q�̔z���p�ӂ��܂��Dbuf_in �ɂ́C�\�[�g���ׂ����̓f�[�^���i�[����܂��D
buf_out�ɂ́C�\�[�e�B���O���ꂽ���ʂ��i�[���܂��D

  18
  19 /*-------------------------------------------------------------------*/
  20 int main(int argc, char *argv[])
  21 {
  22     int i,n;
  23     double time_start, time_end;
  24     swt_handle *h[NUMBER_OF_SPES];
  25     struct argument spe_arg[NUMBER_OF_SPES];
  26     _spe_stdio_init(SPE_STDIO_BUF_SIZE);
  27     _spe_stdio_start();
���C���֐��̍ŏ��̕����ł��D�ϐ��̒�`�ƁCSPE�̕W���o�͂̂��߂̏�������
�����Ȃ��܂��D

  28
  29     n = generate_list(buf_in, buf_out);
�\�[�g���ׂ����͂��擾���܂��D�f�[�^�̐����Ԓl�Ƃ��ē����C�ϐ�n��
�i�[���܂��D

  30
  31     for(i=0; i<NUMBER_OF_SPES; i++){
  32         spe_arg[i].id = i;
  33         spe_arg[i].n  = n;
  34         spe_arg[i].adr_in  = (unsigned long long)&buf_in;
  35         spe_arg[i].adr_out = (unsigned long long)&buf_out;
  36         h[i] = spe_wrap_create_dedicated_thread(EXE_SPE1,
  37                                                 (void*)&spe_arg[i],
  38                                                 SIGNOTIFY_INIT_MODE);
  39     }
SPE�̃v���O�����i�X���b�h�j�𐶐����܂��D���p����SPE�̐��� define.h ��
NUMBER_OF_SPES �Ŏw�肵�܂��D�����ł́C���ׂĂ�SPE��EXE_SPE1�Ŏw�肵��
�v���O���������s����悤�ɋL�q����Ă���_�ɒ��ӂ��Ă��������D
���ꂼ�ꂪ�قȂ�v���O���������s����ꍇ�ɂ́C���̕������C������
�K�v������܂��D

  40
  41     time_start = my_clock();
  42     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);
  43     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
  44     time_end = my_clock();
�\�[�e�B���O�̏������n�܂鎞�̎����� time_start �Ɋi�[���āC�������J�n
���܂��D�����̏I�����܂��āC���ׂĂ�SPE�̏������I��������̎�����
time_end �Ɋi�[���܂��D

  45
  46     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_release_thread(h[i]);
SPE�̃X���b�h�i�v���O�����j��������܂��D

  47
  48     check_list(buf_in, buf_out, N);
  49     printf("[ppe_] elapsed time(sec): %10.6f\n", time_end-time_start);
�\�[�e�B���O�̌��ʂ����������ǂ������m�F���܂��D�������Ȃ��ꍇ�ɂ́C
Wrong �Ƃ������b�Z�[�W���o�͂���܂��D�����āC�o�ߎ��Ԃ��o�͂��܂��D

  50
  51     return 0;
  52 }
  53 /*-------------------------------------------------------------------*/
�����܂ł�PPE�̃��C���֐��ł��D���ׂĂ�SPE�œ����v���O�������N�����āC
SPE�݂̂ŏ����������Ȃ��ꍇ�ɂ́C���̃t�@�C����ύX����K�v������܂���D
����CPPE�Ń\�[�e�B���O�̈ꕔ������������ꍇ�ɂ͓K�؂ȏ�����ǉ�����
���������D���ۂ̃R���e�X�g�ł́CPPE�ɂ��\�[�e�B���O�̏������֎~����
�\��������܂��̂ŁC�ł��邩����SPE�ŏ����������Ȃ��悤�ɂ��Ă��������D

file name: spe1.c
   1 /*-------------------------------------------------------------------*/
   2 /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08      */
   3
   4 #include <stdlib.h>
   5 #include <spe_stdio.h>
   6 #include <spu_mfcio.h>
   7 #include "spe_util.h"
   8 #include "define.h"
   9
  10 #define SPE_BUF_ENTRY (SPE_BUF_SIZE/sizeof(float))
  11
  12 struct argument arg _GALIGN;
  13 float buf[SPE_BUF_SIZE/sizeof(float)] _GALIGN; 
spe1.c ��SPE�ŏ��������v���O�����ł��D
128KB�̃o�b�t�@�i�������j����Ɨ̈�Ƃ��Ċm�ۂ��Ă��܂��D

  14 /*-------------------------------------------------------------------*/
  15 void dma_get(char *src, unsigned long long dst, int size)
  16 {
  17     while(size>0){
  18         int dma_size = (size>DMA_MAX_SIZE) ? DMA_MAX_SIZE : size;
  19         mfc_get(src, dst, dma_size, DMA_TAG, 0, 0);
  20         mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  21         mfc_write_tag_update_all();
  22         mfc_read_tag_status();
  23
  24         size = size - dma_size;
  25         src = src + dma_size;
  26         dst = dst + dma_size;
  27     }
  28 }
DMA�]���������Ȃ�mfc_get�֐��ł́C16KB�𒴂���f�[�^�̓]���������Ȃ�
���Ƃ��ł��܂���D
�����ŁC�傫�ȃf�[�^�����C������������SPE��Local Store�ɓ]�����邽��
�̊֐� dma_get ���������܂����D�傫�ȃf�[�^�̓]���́C16KB�̓]�����J��
�Ԃ��܂��D�f�[�^����M�ƌv�Z�Ƃ̃I�[�o���b�v��ʐM�̍��������d�v�ł��D

  29 /*-------------------------------------------------------------------*/
  30 void dma_put(char *src, unsigned long long dst, int size)
  31 {
  32     while(size>0){
  33         int dma_size = (size>DMA_MAX_SIZE) ? DMA_MAX_SIZE : size;
  34         mfc_put(src, dst, dma_size, DMA_TAG, 0, 0);
  35         mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  36         mfc_write_tag_update_all();
  37         mfc_read_tag_status();
  38
  39         size = size - dma_size;
  40         src = src + dma_size;
  41         dst = dst + dma_size;
  42     }
  43 }
dma_get�Ɠ��l�ɁC�傫�ȃf�[�^��Local Store���烁�C���������ɓ]������
���߂̊֐��ł��D

  44 /*-------------------------------------------------------------------*/
  45 int main(int speid, unsigned long long argv)
  46 {
  47     dma_get((char *)&arg, argv, sizeof(struct argument));
  48     spe_printf("[spe%d] let's sort %d items\n", arg.id, arg.n);
SPE�����s����v���O�����̃��C���֐��ł��D
�܂��́C�\����struct argument�Œ�`�����f�[�^��DMA�]���Ŏ󂯎��܂��D
���̍\���̂ɁC�\�[�g���ׂ��f�[�^�̌��Ȃǂ��i�[����܂��D
arg.id �ɂ́C���ꂼ���SPE�����ʂ��邽�߂̔ԍ�(0�`6)���i�[����܂��D

  49
  50     if(arg.n<=SPE_BUF_ENTRY && arg.id==0){ /*** only spe0 works ***/
���̃v���O�����͕��񉻂��{����Ă��܂���D���Ȃ킿�C������SPE�𗘗p����
�Ƃ��Ă��P��SPE�݂̂��\�[�e�B���O�������Ȃ��܂��D
������SPE�𗘗p���ă\�[�e�B���O�������Ȃ��ɂ́C���ꂼ�ꂪ�قȂ�͈͂�
�����I�Ƀ\�[�e�B���O�������Ȃ��Ƃ������헪�ɂ����񉻂��K�v�ł��D

  51         dma_get((char *)&buf, arg.adr_in, sizeof(float)*arg.n);
�\�[�g���ׂ��f�[�^���擾���܂��D
�f�[�^�̓��C���������� arg.adr_in �������A�h���X�Ɋi�[����Ă��܂��D

  52
  53         { /***** insertion sort, this is very slow sorting! *****/
  54             int i,j;
  55             float temp;
  56             for(i=1; i<arg.n; i++){
  57                 temp =buf[i];
  58                 j=i;
  59                 while(j>0 && buf[j-1]>temp){
  60                     buf[j] = buf[j-1];
  61                     j=j-1;
  62                 }
  63                 buf[j] = temp;
  64             }
  65         }
�������\�[�e�B���O�������Ȃ��Ă���d�v�ȕ����ł��D�����ł͑}���@�ɂ��
�\�[�e�B���O���������Ă��܂��D�������C�}���@�͎��s���Ԃ��x���̂ŁC
�����ȕ���\�[�e�B���O�ɕύX����K�v������܂��D

  66
  67         dma_put((char *)&buf, arg.adr_out, sizeof(float)*arg.n);
�\�[�e�B���O�̌��ʂ����C���������Ɋi�[���܂��D
PPE�̃v���O�����ɂ����� buf_out �̃A�h���X���Carg.adr_out �Ɋi�[�����
����̂ŁC�����������A�h���X�Ƀ\�[�e�B���O�ς݂̌��ʂ�]�����܂��D

  68     }
  69     return 0;
  70 }
  71 /*-------------------------------------------------------------------*/
�ȏ�Ń\�[�X�R�[�h�̐����͏I���ł��D

��������́C�\�[�X�R�[�h������s�t�@�C���𐶐����邽�߂̋K�����L�q����
Makefile�ł��Dmake�R�}���h���Q�Ƃ��܂��D
�t�@�C���\����R���p�C���̍œK���I�v�V�����Ȃǂ�ύX���Ȃ��ꍇ�ɂ́C
����Makefile��ύX����K�v�͂���܂���D
file name: Makefile
   1 #######################################################################
   2 # Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08
   3
   4 #######################################################################
   5 ### Please edit this part!
   6 DIR         = $(HOME)/speed/
   7 IDIR        = /export/home/cell2007/
   8 EXE_FILE    = ~/speed/main
   9 TIMEOUT_SEC = 15
�����̃t�@�C����speed�Ƃ����f�B���N�g���ȊO�Ɋi�[����ꍇ�ɂ́C���
������ύX���܂��D�܂��C���s�̃^�C���A�E�g��15�b�ɐݒ肵�Ă��܂��̂ŁC
����Ȑ��̃f�[�^���\�[�e�B���O����ꍇ�Ȃǂ̓^�C���A�E�g�̎��Ԃ�傫��
���Ă��������D

  10
  11 #######################################################################
  12 SPERE   = -I/usr/local/tsb/tool/spere/include
  13
  14 PPE_OPT = -O3 -Wall -m32
  15 PPE_I   = -I$(IDIR)include/ppe -I$(IDIR)include/common $(SPERE) -I..
  16 PPE_L   = -L$(IDIR)lib -L/usr/local/tsb/tool/spere/lib
  17 PPE_LIB = -lspe_stdio_srv -lspe_rpc_srv -lspere_lib -lspere -lpthread
  18 PPE_CC  = /usr/local/tsb/tool/bin/ppu-gcc
  19 PPE_ADD = $(PPE_OPT) $(PPE_I) $(PPE_L)
  20
  21 SPE_OPT = -O3 -Wall -Wno-main
  22 SPE_I   = -I$(IDIR)include/spe -I$(IDIR)include/common $(SPERE) -I..
  23 SPE_L   = -L$(IDIR)lib -L/usr/local/tsb/tool/spere/lib
  24 SPE_LIB = -lspe_stdio -lspe_rpc
  25 SPE_CC  = /usr/local/tsb/tool/bin/spu-gcc
  26 SPE_ADD = $(SPE_OPT) $(SPE_I) $(SPE_L)
  27
  28 #######################################################################
  29 all:
  30 	make main spe1
�����ŃR���p�C���̂��߂̍œK���I�v�V�����Ȃǂ�ݒ肵�܂��D�ʏ�� -O3 ��
���p���܂��D

  31
  32 #######################################################################
  33 main: main.c lib.c define.h Makefile
  34 	$(PPE_CC) $(PPE_ADD) main.c lib.c -o main $(PPE_LIB)
  35
  36 spe1: spe1.c define.h Makefile
  37 	$(SPE_CC) $(SPE_ADD) spe1.c -o spe1 $(SPE_LIB)
main.c �ȊO�̃t�@�C����C�����̃t�@�C���Ƀv���O�������L�q����ꍇ�Ȃǂ́C
���̕�����ύX���܂��D

  38
  39 #######################################################################
  40 run:
  41 	cellexec -t $(TIMEOUT_SEC) $(EXE_FILE)
  42
  43 #######################################################################
  44 clean:
  45 	rm -f *~ *.o main spe1 spe2 spe3 spe4 spe5 spe6 spe7
  46
  47 text:
  48 	kcat define.h lib.c main.c spe1.c Makefile > Code.txt
  49 #######################################################################

����ŁC�������Ńc�[���L�b�g�̉���͂����܂��ł��D
�����o�O���Ȃǂ� cellspeed2007@arch.cs.titech.ac.jp �܂ł��肢���܂��D

�}���`�R�A�v���O���~���O�R���e�X�g�uCell�X�s�[�h�`�������W2007�v���s�ψ���
http://www.hpcc.jp/sacsis/2007/cell-challenge/
<------------------------------------------------------------------------->
