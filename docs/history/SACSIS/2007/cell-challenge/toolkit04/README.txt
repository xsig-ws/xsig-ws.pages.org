<------------------------------------------------------------------------->
Cell Speed Challenge 2007
�������Ńc�[���L�b�g Version 2007-02-02 (Version 0.4)�̎g�����Ɖ��

�y�T�v�z
���̂������Ńc�[���L�b�g��4��SPE�𗘗p��������ł̃\�[�e�B���O�v���O����
�ł��DCell Speed Challenge 2007�̋K��ۑ蕔��̐��`�Ƃ��Ă����p���������D
7��SPE�̗��p��SIMD�ɂ�鍂�����ȂǗl�X�ȍœK���̗]�n���c����Ă��܂��D
���̃h�L�������g�ɉ����āC����Ń\�[�e�B���O�̎����Ɋւ���PowerPoint�̎���
��p�ӂ��܂����D��������Q�l�ɂ��Ă��������D

�y�K��ۑ�T�v�z
�����̃f�[�^�̌n�񂪓��͂Ƃ��ė^����ꂽ�Ƃ��ɁC���������鏇���ɂ���
�����ĕ��ёւ��鏈�����\�[�e�B���O�ƌĂт܂��DCell�X�s�[�h�`�������W2007
�̋K��ۑ�́u�\�[�e�B���O�v�ł��D
�\�[�e�B���O�ł́C�f�[�^�Ɋ܂܂����בւ��̂��߂ɎQ�Ƃ���鍀�ڂ��L�[��
�Ăт܂��D���̃L�[�͒P���x�̕��������_��(C����ɂ�����float�^)�Ƃ��܂��D
�܂��C�L�[�̒l������(�������l���O���C�傫���l������ɂȂ�悤�ȏ���)��
�Ȃ�悤�Ƀf�[�^����ёւ�����̂Ƃ��܂��D

�y�\�[�e�B���O�Ɋւ���⑫�z
�����̐ݒ�́C�ŏI�I�ȁu�K��ۑ� �ڍׂ���уc�[���L�b�g�v�ɂ����ĕύX��
�Ȃ�\��������܂��D
---------------------------------------------------------------------------
1. �\�[�e�B���O�̓��̓f�[�^�`��

�����̃f�[�^�̌n�񂪓��͂Ƃ��ė^����ꂽ�Ƃ��ɁC���������鏇���ɂ���������
���ёւ��鏈�����\�[�e�B���O�ƌĂт܂��D
�\�[�e�B���O�̑ΏۂƂȂ�1�̃f�[�^��v�f(element)�ƌĂԂ��Ƃɂ��܂��D
���̗v�f�ɂ�1�̃L�[(key)�Ƃ��钷���������X�g(list)���܂܂�܂��D
���X�g�̒�����M�Ƃ����ϐ����邢�͒萔�ŕ\������ƁC�v�f�́CC����̍\���̂�
�p���Ď��̗l�ɕ\���ł��܂��D

struct element{
    float key;
    float list[M];
};

�������Ȍ��ɂ��邽�߂ɍ\���̂𗘗p���ĕ\�����܂������C���ۂɂ́C���̓f�[�^
�ɂ����郊�X�g�̒���M�͌Œ肳�ꂽ�l�ł͂Ȃ��C�v���O�������N�����Ă���^��
���邱�ƂɂȂ�܂��D
---------------------------------------------------------------------------
2. ���̓f�[�^����яo�̓f�[�^�̔z�u

�v���O�����̊J�n���_�ŁC���̓f�[�^�͔z��̐擪����i�[����Ă�����̂�
���Ă��܂��D
�\�[�e�B���O���I���������_�ŁC�o�̓f�[�^�ɂ��Ă��C�\�[�e�B���O���ꂽ���
�Ŕz��̐擪����i�[���邱�ƂƂ��Ă��܂��D���̎��C�z��̎c��̃������ɂ�
�ǂ̂悤�Ȓl���i�[����Ă��Ă����܂��܂���D
---------------------------------------------------------------------------
3. ���X�g�̒����Ɋւ��鐧��

��ɐ��������悤�ɁC�\�[�e�B���O�̗v�f�ɂ�1�̃L�[�Ƃ��钷���������X�g
���܂܂�܂��D�C�ӂ̒����̃��X�g���l����ƁC�f�[�^�]���̐����T�C�Y�Ȃǂ�
�����Ƃ������ώG�ȏ������K�v�ƂȂ�܂��D���̂��߁C1�̗v�f�̃T�C�Y������
�ɂȂ�悤�ɐ���������邱�Ƃɂ��܂��D

sizeof(struct element) = 2 ^ m (�o�C�g)
m = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}

���Ȃ킿�C1�̗v�f�̃T�C�Y��16B����16KB�͈̔͂ŁC16B�̔{�X�ƂȂ�l�ɐ���
���܂��D����ɂ��C1�̗v�f�̓]����DMA�̃T�C�Y�𒴂��邱�Ƃ��Ȃ��Ȃ�C
�v���O���~���O���e�ՂɂȂ�܂��D
---------------------------------------------------------------------------
4. ���X�g����L�[�𓾂邽�߂̌v�Z

���̓f�[�^�͗v�f�̔z��Ƃ��ė^����܂����C�����̃L�[�ɑΉ����镔���̒l
�̓[���ŏ���������Ă��܂��D
���[�U�̓\�[�e�B���O���J�n����O�ɁC���X�g����L�[�̒l�𐶐����Ȃ����
�Ȃ�܂���D
���X�g����L�[���v�Z��������̓v���O�����̊J�n���ɗ^�����܂��D
����2�̂����̂ǂ��炩���w�肳��܂��D

(1)���X�g�Ɋ܂܂�邷�ׂĂ̒l�̍ő�l
key = max{list[j]}, (j=0,1,2, ... , M-1)

(2)���X�g�Ɋ܂܂�邷�ׂĂ̒l��2��̘a
key = list[0]*list[0] + list[1]*list[1] +, ... , list[M-1] * list[M-1]

�L�[�C���X�g�̗v�f�͂Ƃ��ɒP���x�̕��������_���Ƃ��ĕ\������܂��D
�܂��C���ꂼ��̗v�f�̐擪��4�o�C�g�ɃL�[���i�[�������̂Ƃ��C�c���
���X�g���i�[�������̂Ƃ��܂��D
---------------------------------------------------------------------------
5. SPE���󂯂Ƃ���

SPE�̃v���O�����ł́C���Ɏ����悤�ȏ����󂯎�藘�p���邱�Ƃ��ł��܂��D

struct shared_control{
    unsigned int flag;
    unsigned int id;       // SPE�̎��ʔԍ�
    unsigned int spe_num;  // SPE�̌��ŁC7�ɌŒ肳���D
    unsigned int buf_addr; // �z��̐擪�A�h���X
    unsigned int datanum;  // �\�[�e�B���O���ׂ��v�f�̐� N
    unsigned int listnum;  // ���X�g�̒��� M
    unsigned int keytype;  // ���X�g����L�[���v�Z����������w�肷��ϐ��D
    unsigned int padding[25]; // 128�o�C�g�ɐ��񂷂邽�߂ɗ��p���Ă���D
};
---------------------------------------------------------------------------

�y��P�� �������Ńc�[���L�b�g�̎g�����z

Cell�I�����C���T�[�r�X celldev3.celltsb.com �Ƀ��O�C�����Ă��������D
�z�[���f�B���N�g���� speed �Ƃ����f�B���N�g�����쐬���C
�����ɃT���v���t�@�C���R�s�[���܂��D���̂��߂̃R�}���h�͎��̒ʂ�ł��D

$ cd ~
$ mkdir speed
$ cp /export/home/cell2007/share/toolkit04/* .

�R���p�C������ю��s�̂��߂ɂ́C�쐬���� speed �Ƃ����f�B���N�g���Ɉړ�
���āCmake �R�}���h�𗘗p���܂��D���̂��߂̃R�}���h�͎��̒ʂ�ł��D

$ cd ~/speed
$ make
$ make run

�Q�Ԗڂ�make�ŃR���p�C���C�R�Ԗڂ�make run�Ŏ��s���J�n���܂��D

30MB(�f�[�^��983,040�C���X�g��7)�̃f�[�^�ɂ��āC
���X�g���f�[�^��2��̘a���L�[�Ƃ��ă\�[�e�B���O���s���܂��D

�\�[�e�B���O�̌��ʂ��������ꍇ�ɂ́C���̂悤�ȃ��b�Z�[�W�ƌv�Z�ɗv��������
���o�͂��܂��D
[ppe_] * Result Good! *
[ppe_] n 983040, listnum 7, input data size 30 [MByte]
[ppe_] elapsed time(sec) 2.724527

���ʂ��Ԉ���Ă���ꍇ�ɂ� Wrong! �Ƃ��������b�Z�[�W��\�����܂��D

�y��Q�� �������Ńc�[���L�b�g�̃t�@�C���\���z

�쐬�����f�B���N�g���ɂ͎��̃t�@�C�����R�s�[����܂��D

(1) README.txt  : ���̃t�@�C��
(2) Code.txt    : ���ׂẴ\�[�X�R�[�h���܂Ƃ߂��e�L�X�g�t�@�C��
(3) Makefile    : make �R�}���h�����p����t�@�C��
(4) define.h    : ���ׂẴv���O�������C���N���[�h����t�@�C��
(5) main.c      : PPE�̂��߂̃��C���֐����܂�C�v���O����
(6) lib.c       : PPE�̃v���O���������p����C�̊֐��Q
(7) spe1.c      : SPE�̂��߂̃��C���֐����܂�C�v���O����

�ʏ�́C(1), (2), (3), (4), (5), (6) �̃t�@�C����ύX����K�v�͂���܂���D
(7) spe1.c ��ύX���āC�����ȃ\�[�e�B���O�v���O�����̎�����ڎw���Ă��������D

�y��R�� �������Ńc�[���L�b�g�̃v���O��������z

(3) Makefile �̓��e�͈ȑO�̔łƓ����ł��D
�܂��́C���̃f�B���N�g���� README.txt ��ǂނ��Ƃ����߂܂��D
/export/home/cell2007/share/toolkit02/

��������́C�v���O�����̉���ł��D
�������̃X�y�[�X�Ɛ����i�R�[�h�̍s�ԍ��j����n�܂�s�́C�\�[�X�R�[�h��
�ꕔ�ł��D����ȊO�̍s������ł��D

(4) define.h    : ���ׂẴv���O�������C���N���[�h����t�@�C��
(6) lib.c       : PPE�̃v���O���������p����֐��Q
(5) main.c      : PPE�̂��߂̃��C���֐����܂ރv���O����
(7) spe1.c      : SPE�̂��߂̃��C���֐����܂ރv���O����
���̏����Ńv���O�����̃R�[�h���݂Ă����܂��D

********************************************************************************
file name: define.h
   1  /*----------------------------------------------------------------------*/
   2  /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
   3  /*----------------------------------------------------------------------*/
   4  #define NUMBER_OF_SPES 7

���p����SPE�̐����w�肵�܂��D1�`7�̐�����ݒ肵�܂��D
�����ł�7��ݒ肵�Ă��܂��D

   5  #define EXE_SPE1 "speed/spe1"
   6  #define EXE_SPE2 "speed/spe1"
   7  #define EXE_SPE3 "speed/spe1"
   8  #define EXE_SPE4 "speed/spe1"
   9  #define EXE_SPE5 "speed/spe1"
  10  #define EXE_SPE6 "speed/spe1"
  11  #define EXE_SPE7 "speed/spe1"
  12  

�eSPE�Ŏg�p������s�t�@�C�����قȂ�ꍇ�ɂ́C�����Ńt�@�C�������w�肵�܂��D
�������C���̔ł̎����ł́C������SPE�𗘗p����ꍇ�ɂ��S�Ă�SPE�������
�t�@�C��EXE_SPE1�����s����悤�ɋL�q����Ă��܂��D
�قȂ���s�t�@�C�����w�肷�邽�߂ɂ� main.c ��35�s�ڕt�߂̏C�����K�v�ł��D

  13  #define KEY_TYPE 1 // choose 1 or 2
  14  #define LIST_NUM 7 // list size M
  15  #define INPUT_DATA (30 * 1024 * 1024) // input data size
  16  

�L�[�̎Z�o���@�C���X�g���C�f�[�^�T�C�Y���w�肵�܂��D
�����ł́CKEY_TYPE = 1(���X�g�̊e�f�[�^��2��̘a)�C
���X�g����7�C�f�[�^�T�C�Y��30MB���w�肵�Ă��܂��D
���s���ɕp�ɂɃG���[����������ꍇ�ɂ́C�f�[�^�T�C�Y��10MB���x�ɏ���������
�����Ă݂Ă��������D

  17  #define ELEMENT_SIZE (4*(KEY_TYPE + LIST_NUM)) // element size
  18  #define DATA_NUM (INPUT_DATA/ELEMENT_SIZE)     // the number of element N
  19  #define USER_MEM (INPUT_DATA*3)                // user working memory

1�̗v�f�̃T�C�Y�C����їv�f�̐��C���p�\�ȃf�[�^�T�C�Y���v�Z���Ă��܂��D

  20  
  21  /*----------------------------------------------------------------------*/
  22  #define DMA_TAG 0
  23  #define SPE_STDIO_BUF_SIZE 512
  24  #define SPE_DMA_ALIGN (128)

DMA�]���p�̃^�O�ԍ��ɖ��O(DMA_TAG)�����Ă��܂��D
SPE_STDIO_BUF_SIZE�ŁCSPE����̕W���o�͗p�̃o�b�t�@�T�C�Y���w�肵�Ă�
�܂��D
DMA�]���̍ŏ��T�C�Y�ł���128�o�C�g���CSPE_DMA_ALIGN�Ƃ������O��t����
���܂��D

  25  
  26  /*----------------------------------------------------------------------*/
  27  struct argument{
  28      int id;               //
  29      unsigned int sc_addr; //
  30      float padding[30];    // just for padding
  31  };
  32  
  33  struct spe_ctrl{
  34      unsigned int flag;        //
  35      unsigned int id;          //
  36      unsigned int spe_num;     //
  37      unsigned int buf_addr;    //
  38      unsigned int n;           // the number of element N
  39      unsigned int listnum;     // list size M
  40      unsigned int keytype;     //
  41      unsigned int padding[25]; // just for padding
  42  };
  43  /*----------------------------------------------------------------------*/

�\����argument�́CPPE����SPE�ɏ���`���邽�߂ɗ��p���܂��D
�\����argument��SPE���Ƃ�spe_ctrl���擾����ۂ̃A�h���X�������`�����܂��D

�\����spe_ctrl�ł́C�ȉ��̏���SPE���\�[�e�B���O�Ɋւ�������擾��
�邽�߂ɗ��p����܂��D
�\����spe_ctrl�ł́C�ȉ��̏��PPE�ɓ`�����܂��D
�E flag     : �v�Z�J�n�t���O(1�ɂȂ�ƌv�Z���J�n�ł���)
�E id       : SPE�ԍ�(0�`7)
�E spe_num  : ���p�\��SPE�̐�(�ʏ�7) ##### �K�v��������
�E buf_addr : �\�[�e�B���O�Ώۃf�[�^���i�[���ꂽ�������̐擪�A�h���X
�E n        : �v�f��
�E listnum  : ���X�g��
�E keytype  : �L�[�̌v�Z���@

SPE�́C�܂����̍\����argument�̃f�[�^����M���ă\�[�e�B���O��񂪊i�[
���ꂽ�A�h���X���擾���܂��D���̃A�h���X����\����spe_ctrl�̃f�[�^����
�M���C�\�[�e�B���O�Ɋւ�����𓾂܂��D
���̍ہCflag�̒l���`�F�b�N���Ă���C���̒l��0�ȊO�ɂȂ�ƁC�\�[�e�B��
�O��񂪓���ꂽ���ƂɂȂ�C�������J�n����܂��D

�����̍\���̂́CDMA�]���������悭�s�����߂ɁC�\���̂̃T�C�Y��128�o�C
�g�ɂȂ�悤�ɁC�K�؂Ƀp�f�B���O���ݒ肳��Ă��܂��D

********************************************************************************
file name: lib.c
   1  /*----------------------------------------------------------------------*/
   2  /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
   3  /*----------------------------------------------------------------------*/
   4  #include <stdio.h>
   5  #include <stdlib.h>
   6  #include <sys/time.h>
   7  #include <time.h>
   8  #include <spe/sys/spelib_types.h>
   9  #include <spe_stdio_srv.h>
  10  
  11  #include "define.h"
  12  
  13  struct data_block{ // 32 Byte
  14      float key;
  15      float data[LIST_NUM];
  16  };

�`�F�b�N���[�`���Ŏg���\����data_block��錾���Ă��܂��D

  17  
  18  /*----------------------------------------------------------------------*/
  19  double my_clock()
  20  {
  21      struct timeval tv;
  22      gettimeofday(&tv, NULL);
  23      return tv.tv_sec + (double)tv.tv_usec*1e-6;
  24  };

���Ԍv�����[�`��my_clock��錾���Ă��܂��D
���݂�gettimeofday�Ŏ��Ԃ��v�����Ă��܂��D

  25  
  26  /*----------------------------------------------------------------------*/
  27  
  28  void generate_list(float* buf, int datanum, int listnum){
  29      int i,j;
  30      
  31      srand((unsigned int)time(NULL));
  32  //    srand(1);
  33      
  34      for(i = 0; i<datanum; i++){
  35          buf[(listnum+1)*i] = 0.0;
  36          for(j = 0; j<listnum; j++){
  37              buf[(listnum+1)*i+j+1] = (float)rand()/RAND_MAX;
  38          }
  39      }
  40  };

�֐�generate_list�ŗ����̃\�[�e�B���O�Ώۃf�[�^�����|�C���^buf�Ɋi�[���܂��D
�f�[�^�̓L�[�i�[�p��buf[(listnum+1)*i]�ɂ�0.0���i�[���C�L���ȃf�[�^�͓���
�Ă��܂���D

  41  
  42  /*----------------------------------------------------------------------*/
  43  float square_norm(float* n, int listnum){
  44      int i;
  45      float norm = 0.0;
  46      for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
  47      return norm;
  48  };

square_norm�̓`�F�b�N���[�`���Ŏg�p����C�z��̃|�C���^n����listnum��
��float�f�[�^��2��l�̘a��Ԃ��֐��ł��D

  49  
  50  /*----------------------------------------------------------------------*/
  51  void check_list(float* buf,int datanum, int listnum, int keytype){
  52      int i;
  53      struct data_block* ssblock;
  54      
  55      ssblock = (struct data_block*)buf;
  56      for(i=1;i<datanum;i++){
  57          if(ssblock[i - 1].key > ssblock[i].key){
  58              printf("[ppe_] * Wrong! * n[%d]:k1 %5.10f n[%d] k2 %5.10f\n",
  59                     i-1, ssblock[i - 1].key, i, ssblock[i].key);
  60              return;
  61          }
  62      }
  63      printf("[ppe_] * Result Good! *\n");
  64  }
  65  /*----------------------------------------------------------------------*/

�\�[�e�B���O���������Ă��邱�Ƃ��m�F����check_list�֐��ł��D
�\���̂ւ̃|�C���^ssblock�ɁC�\�[�e�B���O�z��̐擪���i�[���Ă����C
float�^�̈ꎟ���z����C�\����data_block�Ƃ��ėv�f�ɊȈՂɃA�N�Z�X�ł�
��悤�ɂ��܂��D
�v�f�̃L�[�̒l���r���C�����ɕ���ł��邱�Ƃ��m�F���܂��D

********************************************************************************
file name: main.c
   1  /*----------------------------------------------------------------------*/
   2  /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
   3  /*----------------------------------------------------------------------*/
   4  #include <stdio.h>
   5  #include <spe/sys/spelib_types.h>
   6  #include <spe_stdio_srv.h>
   7  #include "spe_wrap.h"
   8  #include "ppe_util.h"
   9  #include "define.h"
  10  #include <float.h>
  11  #include <fenv.h>
  12  #include <spere_cbea.h>
  13  
  14  /*----------------------------------------------------------------------*/
  15  double my_clock();
  16  void generate_list(float*, int, int);
  17  void check_list(float*, int, int, int);

lib.c���񋟂����֐��ł��D
�ォ��C
�Emy_clock()      : ���Ԍv�����[�`��
�Egenerate_list() : �\�[�e�B���O�Ώۃf�[�^�������[�`��
�Echeck_list()    : �`�F�b�N���[�`��

  18  
  19  volatile static float buf[USER_MEM/4] _ALIGN;
  20  volatile static struct argument sarg[NUMBER_OF_SPES] _ALIGN;
  21  volatile static struct spe_ctrl ctrl[NUMBER_OF_SPES] _ALIGN;
  22  double t1,t2; // start and end time stamp

main�֐����Ŏg���ϐ��Q�ł��D
�z��͂��ꂼ��_ALIGN�Ő擪��128�o�C�g���E�ɐ��񂳂�Ă��܂��D
�Efloat buf[]          : �\�[�e�B���O�ΏۂƂȂ�f�[�^���i�[�����z��
�Estruct argument sarg : PPE����SPE�ɓ`�����񂪊i�[�����\����(define.h)
                         ���̍\���̂�SPE��main�֐��̈����ɂȂ�܂�
�Estruct spe_ctrl ctrl : SPE���擾����\�[�e�B���O�Ɋւ����񂪊i�[�����z��(define.h)
�Edouble t1, t2        : ���Ԍv���̎n�܂�ƏI���̒l���i�[�����ϐ�


  23  
  24  /*----------------------------------------------------------------------*/
  25  void ppe_sort(volatile float* buf, int n, int listnum, int keytype){

ppe_sort�֐��ŁC�\�[�e�B���O�����s���܂��D
�܂��CSPE�X���b�h�̐�����A�N�Z�X�q���g�̐ݒ�C�\�[�e�B���O���̐ݒ�Ȃǂ��s���܂��D
���̌�CSPE�X���b�h�����s���CSPE�X���b�h���I������܂ő҂��܂��D

�����́C�ȉ���4�ł��D
�Evolatile float* buf : �\�[�e�B���O�Ώۃf�[�^���i�[���ꂽ�������̃|�C���^
�Eint n               : �v�f��
�Eint listnum         : ���X�g��
�Eint keytype         : �L�[�̌v�Z���@�C

  26     int i;
  27     swt_handle *h[NUMBER_OF_SPES];
  28     spere_object_id hint[NUMBER_OF_SPES][3];

�X���b�h�n���h��(swt_handle) h����уA�N�Z�X�q���g(spere_object_id)
hint��錾���܂��D

  29  
  30     _spe_stdio_init(SPE_STDIO_BUF_SIZE);
  31     _spe_stdio_start();

SPE�̕W���o�͂��󂯕t����T�[�o���N�����܂��D
1���spe_printf�ɂ��CSPE_STDIO_BUF_SIZE(512Byte)�Œ�`���ꂽ�T�C�Y���C
�W���o�͂𓾂邱�Ƃ��ł��܂��D
�������Cspe_printf�ŏo�͂��镶����SPE_STDIO_BUF_SIZE�𒴂����ꍇ�C�o
�͂�0�ɂȂ��Ă��܂��܂��D

  32  
  33     for(i=0; i<NUMBER_OF_SPES; i++){
  34         h[i] = spe_wrap_create_dedicated_thread(
  35             EXE_SPE1, (void*)&sarg[i], SIGNOTIFY_INIT_MODE);
  36         sarg[i].id      = i;
  37         sarg[i].sc_addr = (unsigned int)&ctrl[i];
  38     }

�ȉ��̏����ł́CSPE�̐������C���������s����܂��D
�X���b�h�n���h��h�ɁC�ȉ��̏�����SPE�X���b�h�𐶐����Đݒ肵�܂��D
�ESPE[i]�Ŏ��s�������s�o�C�i���Ƃ���EXE_SPE1�Ŏw�肳�ꂽ�t�@�C�����w��
  (defie.h�E��΃p�X)
�E������sarg[i]�̃A�h���X���w��

SPE�ɗ^�������sarg�Ɉȉ��̃����o�ϐ���ݒ肵�C�eSPE����Q�Ƃł���悤�ɂ��܂��D
�Eint id               : SPE�ԍ�
�Eunsigned int sc_addr : ���ꂼ���ctrl(�\����spe_ctrl�^)�̃A�h���X

  39  
  40     for(i=0; i<NUMBER_OF_SPES; i++){
  41         ctrl[i].flag     = 0;
  42         ctrl[i].id       = i;
  43         ctrl[i].buf_addr = (unsigned int)buf;
  44         ctrl[i].n        = n;
  45         ctrl[i].listnum  = listnum;
  46         ctrl[i].spe_num  = NUMBER_OF_SPES;
  47     }

�\�[�e�B���O���spe_ctrl�ɁC�K�v�Ȓl(define.h�̍\����spe_ctrl�̉���Q
��)��ݒ肵�܂��D

  48  
  49     for(i=0; i<NUMBER_OF_SPES; i++){
  50         hint[i][0] = spe_wrap_add_access_hint(
  51             h[i]->id, (void*)&sarg[i], SPE_DMA_ALIGN);
  52         hint[i][1] = spe_wrap_add_access_hint(
  53             h[i]->id, (void*)&ctrl[i], SPE_DMA_ALIGN);
  54         hint[i][2] = spe_wrap_add_access_hint(
  55             h[i]->id, (void*)buf, USER_MEM);
  56     }

struct argument sarg[7]�Cstruct spe_control sarg[7], buf ���ꂼ��A�N
�Z�X�q���g��ݒ肵�܂��D

  57  
  58     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);

SPE�X���b�h�����s���܂��D

  59  
  60     /**** start the sorting *****/
  61     t1 = my_clock();
  62     for(i=0; i<NUMBER_OF_SPES; i++) ctrl[i].flag = 1;

���Ԍv�����J�n���Cctrl��flag��1���i�[���CSPE�v���O�����Ń\�[�e�B���O��
�������J�n���܂�(spe1.c�Cmain�֐��Q��)�D

  63     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
  64     t2 = my_clock();
  65     /**** end   the sorting *****/

�S�Ă�SPE�v���O�������I������܂ő҂��܂��D
���Ȃ킿�CPPE���ł�SPE�Ɏ��s���w������݂̂ŁC�\�[�e�B���O�Ɋւ��鏈��
�͍s���܂���D
�I��������C���Ԍv�����I�����܂��D

  66  
  67     for(i=0; i<NUMBER_OF_SPES; i++){
  68         spe_wrap_remove_access_hint(h[i]->id, hint[i][0]);
  69         spe_wrap_remove_access_hint(h[i]->id, hint[i][1]);
  70         spe_wrap_remove_access_hint(h[i]->id, hint[i][2]);
  71         spe_wrap_release_thread(h[i]);
  72     }

�A�N�Z�X�q���g���������C�X���b�h��������܂��D

  73     _spe_stdio_end();
  74  /*
  75     {
  76         int j;
  77         for(i=0;i<n;i++){
  78             printf("%4d : key %3.3f : ",i,buf[i*(listnum+1)]);
  79             for(j=0;j<listnum;j++){
  80                 printf("%3.3f ",buf[i*(listnum+1)+j+1]);
  81             }
  82             printf("\n");
  83         }
  84     }
  85  */
  86  };

SPE�̕W���o�͂��󂯕t����T�[�o���~���܂��D
ppe_sort�֐����I�����܂��D

  87  
  88  /*----------------------------------------------------------------------*/
  89  int main(int argc, char *argv[])

PPE�Ŏ��s�����main�֐��ł��D

  90  {
  91     int n       = DATA_NUM;
  92     int listnum = LIST_NUM;
  93     int keytype = KEY_TYPE;
  94  
  95     if(argc == 4){
  96         n       = atoi(argv[1]);
  97         listnum = atoi(argv[2]);
  98         keytype = atoi(argv[3]);
  99     }

�W�����͂���C�v�f���C���X�g���C�L�[�̌v�Z���@���擾���܂��D
��L���ݒ肳��Ă��Ȃ��ꍇ�́Cdefine.h���炻�ꂼ��̒l��ݒ肵�܂��D

 100  
 101     generate_list((float*)buf, n, listnum);
 102     ppe_sort((volatile float*)buf, n, listnum, keytype);
 103     check_list((float*)buf, n, listnum, keytype);

generate_list��buf�ɗv�f��n�C���X�g��listnum�̑Ώۃf�[�^�z��𐶐����܂��D
ppe_sort��SPE�X���b�h�����s���C�\�[�e�B���O���s���܂��D
���̌��ʂ�check_list�Ō��؂��܂��D

 104  
 105     printf("[ppe_] n %d, listnum %d, input data size %d [MByte]\n",
 106            n, listnum, sizeof(float)*n*(listnum+1)/1024/1024);
 107     printf("[ppe_] elapsed time(sec) %8.6f\n", t2-t1);
 108     return 0;
 109  }
 110  /*----------------------------------------------------------------------*/

�\�[�e�B���O�Ώۃf�[�^�T�C�Y�Ȃǂ�\�����C�v�����ꂽ�v�Z���Ԃ�\������
�I�����܂��D

********************************************************************************
SPE�Ŏ��s�����v���O�����ɂ��ĉ�����܂��D
file name: spe1.c
   1  /*----------------------------------------------------------------------*/
   2  /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
   3  /*----------------------------------------------------------------------*/
   4  #include <spu_intrinsics.h>
   5  #include <spelib.h>
   6  #include <spe_stdio.h>
   7  #include <stdlib.h>
   8  #include <spu_mfcio.h>
   9  #include "spe_util.h"
  10  #include "define.h"
  11  #include <float.h>
  12

SPE�v���O�����Ŏg�p����w�b�_�t�@�C���ł��D

  13  /*----------------------------------------------------------------------*/
  14  struct data_block{ // 32 Byte
  15      float key;
  16      float data[LIST_NUM];
  17  };

�\����data_block�ł��D�L�[�Ɛ��l�̃��X�g����Ȃ�܂��D������g���āC��
�C������������f�[�^������Ă�����C�������񂾂肵�܂��D
���݂̂Ƃ���́C���X�g��LIST_NUM��7�ŌŒ�Ȃ̂ŁCstruct data_block�̃T�C�Y��
32Byte�ɂȂ�܂��D

  18
  19  struct transfer_block{ // 128 Byte
  20      struct data_block dblock[4];
  21  };
  22

�\����transfer_block�ł��DCell Broadband Engine��DMA�]���̒P�ʂ�128�o
�C�g���ƂȂ̂ŁC128�o�C�g��struct data_block(32�o�C�g)��4���ׂ܂��D

  23  struct sync_block{
  24      unsigned int status[32];
  25  };
  26

�\����sync_block�ł��Dstatus�Ƃ������O��unsigned int�^�z��������C����
���g����SPE�Ԃ̓��������܂��D

  27  struct merge_return{
  28      int oa;
  29      int ob;
  30  };
  31  /*----------------------------------------------------------------------*/

�\����merge_return�ł��D�}�[�W�\�[�g�ɗ��p���܂��D

  32
  33  volatile static struct argument arg _GALIGN;
  34  volatile static struct spe_ctrl sc _GALIGN;
  35  volatile static struct sync_block sb _GALIGN;
  36  volatile static struct transfer_block eb _GALIGN;
  37

�e��O���[�o���ϐ���錾���Ă��܂��D
�S�ẴO���[�o���ϐ���_GALIGN������p���C128�o�C�g���E�ɐ��񂳂�Ă��܂��D
�Earg : �����Ƃ��ė^������\���̂ł�
�Esc  : PPE����\�[�e�B���O�Ɋւ�������擾����\���̂ł�
�Esb  : SPE�Ԃ̓��������ۂɗ��p����\���̂ł�
�Eeb  : ���C����������SPE��̃��[�J���������̒ʐM�ɗ��p����\���̂ł�

  38  /*----------------------------------------------------------------------*/
  39  void dmaget(void* d, unsigned long long addr, unsigned int size){
  40      /***** DMA Data Transfer, Memory -> LS *****/
  41      mfc_get(d, addr, size, DMA_TAG, 0, 0);
  42      mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  43      mfc_write_tag_update_all();
  44      mfc_read_tag_status();
  45  }
  46

���C������������SPE�̃��[�J���X�g���[�W(LS)�փf�[�^��]������dmaget��
���ł��D����addr�ŗ^����ꂽ���C���������̃A�h���X����Csize�o�C�g�̃f�[
�^���|�C���^d�Ŏ����ꂽLS�̃������֓]�����܂��D
addr�Ŏ������A�h���X��128�o�C�g���E�ŁCsize��128�o�C�g���Ƃ̓]������
�����ǂ����Ƃ��m���Ă��܂��D
dmaget�֐���mfc_get�Ƃ���DMA�]���p�̊֐�����񂾂��ĂԂ��߁Csize
�̍ő�l��16384�o�C�g(16KB)�ɐ�������܂��D

  47  /*----------------------------------------------------------------------*/
  48  void dmaput(void* d, unsigned long long addr, unsigned int size){
  49      /***** DMA Data Transfer, LS -> Memory *****/
  50      mfc_put(d, addr, size, DMA_TAG, 0, 0);
  51      mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  52      mfc_write_tag_update_all();
  53      mfc_read_tag_status();
  54  }

SPE��LS���烁�C���������փf�[�^��]������dmaput��
���ł��D
����addr�ŗ^����ꂽ���C���������̃A�h���X�ցCsize�o�C�g�̃f�[
�^���|�C���^d�Ŏ����ꂽLS�̃������̓��e��]�����܂��D
dmaget�֐��Ɠ��l�ɁC
addr�Ŏ������A�h���X��128�o�C�g���E�ŁCsize��128�o�C�g���Ƃ̓]������
�����ǂ����Ƃ��m���Ă��܂��D
dmaget�֐���mfc_get�Ƃ���DMA�]���p�̊֐�����񂾂��ĂԂ��߁Csize
�̍ő�l��16384�o�C�g(16KB)�ɐ�������܂��D

  55  /*----------------------------------------------------------------------*/
  56  float square_norm(float* n, int listnum){
  57      int i;
  58      float norm = 0.0;
  59      for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
  60      return norm;
  61  };
  62

square_norm��SPE�v���O�����Ŏg�p����C�z��̃|�C���^n����listnum��
��float�f�[�^��2��l�̘a��Ԃ��֐��ł��D

  63  /*----------------------------------------------------------------------*/
  64  unsigned int get_eaddr(unsigned int saddr, int eid){
  65      unsigned int eaddr;
  66      eaddr = saddr + eid * 128;
  67      return eaddr;
  68  };
  69

get_eaddr�́C128�o�C�g��1�u���b�N�Ƃ��āCsaddr�ŗ^������A�h���X����
eid�u���b�N�ڂ̃A�h���X��Ԃ��֐��ł��D

  70  /*----------------------------------------------------------------------*/
  71  struct transfer_block get_tblock(int eid, struct spe_ctrl* sc){
  72      unsigned int eaddr; // element address
  73
  74      eaddr = get_eaddr(sc->buf_addr,eid);
  75      dmaget((void*)&eb, eaddr, sizeof(struct transfer_block));
  76
  77      return eb;
  78  };
  79
  86

get_tblock�́C128�o�C�g��1�u���b�N�Ƃ��āCsaddr�ŗ^������A�h���X����
eid�u���b�N�ڂ���128�o�C�g�̃f�[�^���擾����֐��ł��D
�Ԓl��struct transfer_block�^�ɂȂ�܂��D

  80  /*----------------------------------------------------------------------*/
  81  void put_tblock(int eid, struct transfer_block* eblock, struct spe_ctrl* sc){
  82      unsigned int eaddr; // element address
  83      eaddr = get_eaddr(sc->buf_addr,eid);
  84      dmaput((void*)eblock, eaddr, sizeof(struct transfer_block));
  85  };

put_tblock�́Csaddr�ŗ^�����郁�C���������̃A�h���X�ɁC128�o�C�g��1
�u���b�N�Ƃ���eid�u���b�N�ڂ�128�o�C�g�̃f�[�^���������ފ֐��ł��D

  87  /*----------------------------------------------------------------------*/
  88  struct data_block get_data(int id, struct spe_ctrl* sc){
  89      unsigned int eid;
  90      unsigned int eoffset;
  91      struct transfer_block eblock;
  92
  93      eid     = id / 4;
  94      eoffset = id % 4;
  95      eblock = get_tblock(eid, sc);
  96
  97      return eblock.dblock[eoffset];
  98  };
  99

get_data�́Cid�Ԗڂ̗v�f�����C��������������o���֐��ł��D

DMA�]����128�o�C�g�P�ʂł���C�v�f�̃T�C�Y��32�o�C�g�ł��邱�Ƃ���C1
�̗v�f��_����DMA�]�����邱�Ƃ͍���ł��D���̂��߁C128�o�C�g��1�P��
�Ƃ���DMA�]�����s���C�v�f�͂��̌�Ɏ��o�����@�����܂��D

 100  /*----------------------------------------------------------------------*/
 101  void swap_db(int id1, int id2, struct spe_ctrl* sc){
 102      struct transfer_block eblock[2];
 103      unsigned int eid[2];
 104      unsigned int eoffset[2];
 105      struct data_block tmp_db;
 106
 107      eid[0]     = id1 / 4;
 108      eoffset[0] = id1 % 4;
 109
 110      eid[1]     = id2 / 4;
 111      eoffset[1] = id2 % 4;
 112
 113      if(eid[0] == eid[1]){
 114          eblock[0] = get_tblock(eid[0], sc);
 115          tmp_db                       = eblock[0].dblock[eoffset[0]];
 116          eblock[0].dblock[eoffset[0]] = eblock[0].dblock[eoffset[1]];
 117          eblock[0].dblock[eoffset[1]] = tmp_db;
 118          put_tblock(eid[0], &eblock[0], sc);
 119      }
 120      else{
 121          eblock[0] = get_tblock(eid[0], sc);
 122          eblock[1] = get_tblock(eid[1], sc);
 123
 124          tmp_db                       = eblock[0].dblock[eoffset[0]];
 125          eblock[0].dblock[eoffset[0]] = eblock[1].dblock[eoffset[1]];
 126          eblock[1].dblock[eoffset[1]] = tmp_db;
 127
 128          put_tblock(eid[0], &eblock[0], sc);
 129          put_tblock(eid[1], &eblock[1], sc);
 130      }
 131  };
 132

swap_db�́C���C�����������id1�Ԗڂ̗v�f�ƁCid2�Ԗڂ̗v�f�����������
���ł��D
DMA�]����128�o�C�g�P�ʂȂ̂ŁC���ꂼ��̗v�f���܂�128�o�C�g�̃u���b�N
��ǂݍ��݁C�v�f���������āC�����߂��C�Ƃ����菇�����܂��D
id1��id2�̗v�f�������128�o�C�g�u���b�N�ɑ��݂���ꍇ�̏������C

 113      if(eid[0] == eid[1]){

�ŋL�q����Ă��܂��D

 133  /*----------------------------------------------------------------------*/
 134  void qs_sort(int s, int e, struct spe_ctrl* sc){
 135      int l,r;
 136      struct data_block p;
 137      struct data_block fl,fr;
 138
 139      p = get_data(((s+e)/2),sc);
 140      l = s;
 141      r = e;
 142
 143      fl = get_data(l,sc);
 144      fr = get_data(r,sc);
 145
 146      while(1){
 147          while(fl.key < p.key){
 148              l++;
 149              fl = get_data(l,sc);
 150          }
 151          while(fr.key > p.key){
 152              r--;
 153              fr = get_data(r,sc);
 154          }
 155
 156          if(l >= r)
 157              break;
 158
 159          swap_db(l, r, sc);
 160
 161          l++;
 162          r--;
 163          fl = get_data(l,sc);
 164          fr = get_data(r,sc);
 165      }
 166
 167      if(l - s > 1) qs_sort(s, l-1, sc);
 168      if(e - r > 1) qs_sort(l, e, sc);
 169  };
 170

qs_sort�́C�����ŗ^����ꂽs�Ԗڂ̗v�f����Ce�Ԗڂ̗v�f���N�C�b�N�\�[
�g�Ń\�[�e�B���O����֐��ł��D
�v�f�ɂ͂��炩���߃L�[���v�Z����C�i�[����Ă��邱�Ƃ�O��Ƃ��܂��D
s��e�̒��Ԃ̗v�f���s�{�b�g�Ƃ��Đݒ肵�Ă��܂��D

 171  /*----------------------------------------------------------------------*/
 172  void sort_phase1(struct spe_ctrl* sc){

sort_phase1�́C�eSPE���L�[���v�Z���C�����z��̃\�[�e�B���O���s���֐��ł��D

 173      int i,j;
 174      int elnum; // element number
 175      unsigned int eaddr;
 176      unsigned int s;
 177      unsigned int e;
 178
 179      elnum = sc->n / 16;

����̃v���O�����ł́CSPE��4�g���̂ŁC�Ώۂ̗v�f��4��Ԃɕ������C
4��SPE�ŁC�e��Ԃ̗v�f�̃L�[���v�Z���܂��D

 180
 181      // key calculation
 182      for(i=0;i<elnum;i++){
 183          eaddr = get_eaddr(sc->buf_addr, elnum * sc->id + i);
 184          dmaget((void*)&eb, eaddr, sizeof(struct data_block)*4);
 185          for(j=0;j<4;j++){
 186              eb.dblock[j].key = square_norm((float*)eb.dblock[j].data,sc->listnum);
 187          }
 188          dmaput((void*)&eb, eaddr, sizeof(struct data_block)*4);
 189      }

182�s�ڂ���189�s�ڂŁC�e�v�f�̃L�[���v�Z���܂��D
�e��Ԃł́C128�o�C�g�̒���4�̗v�f���i�[����Ă���̂ŁC
�L�[�̌v�Z�̂��߂�DMA�]���̉񐔂�(�v�f����16)��ɂȂ�܂��D
�v�f��128�o�C�g�u���b�N���ƂɎ擾���ăL�[���v�Z���C�v�f�ɃL�[���i�[��
����C�����ꏊ�ɏ����߂��Ă��܂��D

 190
 191      // Sub sorting in a divided elements
 192      s = sc->id * elnum * 4;
 193      e = s + (elnum * 2);
 194      qs_sort(s,e-1,sc);
 195
 196      s = (sc->id * elnum * 4) + (elnum * 2);
 197      e = s + (elnum * 2);
 198      qs_sort(s,e-1,sc);

192�s�ڂ���198�s�ڂ̏����́C���ꂼ���SPE���S������v�f��O���C�㔼��
�������C�����\�[�g���s���Ă��܂��D
�܂�C�\�[�e�B���O�ΏۂƂȂ�v�f�S�̂�8�̋�Ԃɕ������C�eSPE��2��
�̋�Ԃ����ꂼ�ꕔ���\�[�g���邱�ƂɂȂ�܂��D

 199  };
 200

 201  /*----------------------------------------------------------------------*/
 202  void merge_core(struct spe_ctrl* sc, struct merge_return* mr,
 203                  int sa, int sanum, int sb, int sbnum, int da, int danum){

merge_core�́Csa�Ԗڂ̗v�f�����sb�Ԗڂ̗v�f����C
���ꂼ��sanum�Csbnum�̗v�f�����o���C�}�[�W�\�[�g����֐��ł�
(merge_sort_sub�֐����Q��)�D
���̍ہCstruct merge_return* mr�Ŏw�肳�ꂽ�I�t�Z�b�goa�Cob���K�p����
�܂��D
oa�Cob�͌����������sanum�Csbnum�̂����C�\�[�g�̂��߂Ɏg�p�ς݂̗v�f��
�������Ă��邱�ƂɂȂ�܂��D

���ʂ́Cda�Ԗڂ̋�悩��danum�������܂�܂��D
sanum = sbnum = danum �ł��邱�Ƃ�O��ɂ��Ă��邽�߁C2��
���͗��S�ď�������O�ɁC�o�͗񂪂����ς��ɂȂ��Ă��܂��܂��D

���̂��߁C�o�͂������ς��ɂȂ����Ƃ��ɂ́Csa,sb�œǂݏo�����̗v�f�ԍ����C
struct merge_return* mr�Ɋi�[���ĕԂ��܂��D

 204      int i,j,oa,ob;
 205      struct data_block dba,dbb;
 206      struct transfer_block eblock;
 207      int d;
 208
 209      d  = da / 4;

�������݂�128�o�C�g�̓]���u���b�N�P�ʂŏ������݂��s���܂��D
�ǂݏo���͗v�f���Ƃɓǂݏo��������̂ŁC���ӂ��Ă��������D

 210      oa = mr->oa;
 211      ob = mr->ob;
 212
 213      i = 0;
 214      j = 0;
 215      while(i != danum / 4){
 216          if(oa == sanum){
 217              eblock.dblock[j] = get_data(sb + ob, sc);
 218              ob++;
 219          }
 220          else if(ob == sbnum){
 221              eblock.dblock[j] = get_data(sa + oa, sc);
 222              oa++;
 223          }

sa�܂���sb�̗v�f�z�񂪑S�ēǂݏo���I����Ă�����C�������
�v�f�z����o�͗�ɓǂݍ��݂܂��D

 224          else{
 225              dba = get_data(sa + oa, sc);
 226              dbb = get_data(sb + ob, sc);
 227              if(dba.key > dbb.key){
 228                  eblock.dblock[j] = dbb;
 229                  ob++;
 230              }
 231              else{
 232                  eblock.dblock[j] = dba;
 233                  oa++;
 234              }
 235          }

2�̗v�f�z�񂩂�1���v�f��ǂݍ��݁C�L�[���r���܂��D
�L�[�����������̗v�f���o�͗�ɓǂݍ��݂܂��D

 236
 237          j++;
 238          if(j == 4){
 239              put_tblock(d,&eblock,sc);

�o�͗��4�̗v�f�����܂邲�ƂɁCda�̗v�f�z��ɏ����o���܂��D

 240              d++;
 241              i++;
 242              j = 0;
 243          }
 244      }
 245
 246      mr->oa = oa;
 247      mr->ob = ob;

sa,sb�œǂݏo�����̗v�f�ԍ����Cstruct merge_return* mr�Ɋi�[���ĕԂ��܂��D

 248  }


 250  /*----------------------------------------------------------------------*/
 251  void merge_sort_sub(struct spe_ctrl* sc, int sa, int sanum,int sb,
 252                      int sbnum,int da, int danum,int db, int dbnum){

merge_sort_sub�́Csa�Ԗڂ̗v�f�����sb�Ԗڂ̗v�f����C
���ꂼ��sanum�Csbnum�̗v�f�����o���C�}�[�W�\�[�g����֐��ł��D
���ʂ�da�Ԗڂ̗v�f����danum�Cdb�Ԗڂ̗v�f����dbnum�̗v�f�ɏ����o��
��܂��D

 253      struct merge_return mr;
 254
 255      mr.oa = 0;
 256      mr.ob = 0;
 257
 258      merge_core(sc, &mr, sa, sanum, sb, sbnum, da, danum);
 259      merge_core(sc, &mr, sa, sanum, sb, sbnum, db, dbnum);

struct merge_return mr��}��Ƃ��āCda�p��db�p�ɁC2���merge_core�֐����Ăт܂��D

 260  };
 261

 262  /*----------------------------------------------------------------------*/
 263  struct sync_block get_status(int id, struct spe_ctrl* sc){
 264      dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 265      return sb;
 266  };
 267

get_status��id�Ŏ����ꂽSPE�ԍ�������SPE�̏�Ԃ�ǂݍ��ފ֐��ł��D
�eSPE�̏�Ԃ́Cspe_ctrl�ŗ��p�\�ȃ��C���������̏I�[���珇��128�o�C�g
�P�ʂŊi�[����Ă��܂��D
�܂�C
id�� 0 ��SPE�̏�Ԃ� sc->buf_addr + USER_MEM - 128 ����128�o�C�g�C
id�� 1 ��SPE�̏�Ԃ� sc->buf_addr + USER_MEM - 256 ����128�o�C�g�C
�Ƃ����悤�Ɋi�[����Ă��܂��D

 268  /*----------------------------------------------------------------------*/
 269  void put_status(int id, int sid, int status, struct spe_ctrl* sc){
 270      dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 271      sb.status[sid] = status;
 272      dmaput((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 273  };
 274

put_status��id�Ŏ����ꂽSPE�ԍ�������SPE�̏�Ԃ�����������֐��ł��D
��{�I�ɁC�eSPE�͎�����id�̏�Ԃ݂̂̏����������������ƂŁC��Ԃ̈��
����ێ����܂��D

 275  /*----------------------------------------------------------------------*/
 276  void merge_sort(struct spe_ctrl* sc, int direction, int isa, int isb){

merge_sort�́C8�̋�Ԃɕ������ꂽ�\�[�e�B���O�Ώ۔z��ɂ��āC
isa�Ԗڂ̋�Ԃ�isb�Ԗڂ̋�Ԃ��}�[�W�\�[�g����֐��ł��D
�}�[�W�\�[�g�ł��邽�߁C�Ώ۔z��Ɠ����傫���̈ꎞ�ۑ��̈�փ\�[�g��
�ʂ��o�͂��܂��D
�܂��C�}�[�W�\�[�g�͕����؂�ւ����邽�߁C
���i�[�̈�̃f�[�^���}�[�W�\�[�g���Ĉꎞ�ۑ��p�̃������̈�֏o�͂���C
�ꎞ�ۑ��̈�̃f�[�^���}�[�W�\�[�g���Ė��i�[�̈�֏o�͂���C
�Ƃ���2��ނ̕������w�肷�邽�߂ɁCdirection�Ƃ����t���O�𗘗p���܂��D
����́C�Ώ۔z��̖����ȍ~�ɏo�͂���悤�ɂ��Ă��܂��D

 277      int elnum; // element number
 278      int sa,sb,da,db;
 279
 280      elnum = sc->n / 8;
 281
 282      if(direction == 0){
 283          sa = elnum * isa;
 284          sb = elnum * isb;
 285          da = elnum * (isa+8);
 286          db = elnum * (isb+8);

���ۂ̃}�[�W�\�[�g�̂��߂̈�����ݒ肷��D
���i�[�̈�̃f�[�^���}�[�W�\�[�g���Ĉꎞ�ۑ��p�̃������̈�֏o�͂���ꍇ�D
isa�Ԗڂ̋��ƁCisb�Ԗڂ̋����}�[�W�\�[�g���āC
(isa+8)�Ԗڂ̋���(isb+8)�Ԗڂ̋��Ɍ��ʂ������o���D

 287      }
 288      else{
 289          sa = elnum * (isa+8);
 290          sb = elnum * (isb+8);
 291          da = elnum * isa;
 292          db = elnum * isb;

�ꎞ�ۑ��p�̃������̈悩��ǂݏo�����f�[�^���}�[�W�\�[�g���Ė��i�[�̈�֏o�͂���ꍇ�D
(isa+8)�Ԗڂ̋���(isb+8)�Ԗڂ̋����}�[�W�\�[�g���āC
isa�Ԗڂ̋��ƁCisb�Ԗڂ̋��Ɍ��ʂ������o���D

 293      }
 294
 295      merge_sort_sub(sc, sa, elnum, sb, elnum, da, elnum, db, elnum);

�ݒ肳�ꂽ������p���ă}�[�W�\�[�g���s���D

 296  };

 298  /*----------------------------------------------------------------------*/
 299  void wait_spe(struct spe_ctrl* sc, int sid, int status){

wait_spe�͊eSPE�̓�������邽�߂̊֐��ł��D
�֐������s�����ƁC4��SPE�̏�Ԃ����Ԃɓǂݏo���C
�S�Ă�status�ŗ^����ꂽ�ɂȂ�܂ő҂��܂��D

wait_spe�֐����g���āCSPE�Ԃ̓�������邱�ƂɂȂ�܂��D

sid��struct sync_block�̃����o�ł���status[32]�̉��Ԗڂ�
�����̂��߂̏�ԂƂ��ĎQ�Ƃ��邩���w�肵�܂��D

 300      int ret;
 301      int i;
 302      int use_spe_num = 4;
 303      struct sync_block sync;
 304
 305      do{
 306          ret = 0;
 307          for(i=0;i<use_spe_num;i++){
 308              sync = get_status(i,sc);
 309              if(sync.status[sid] != status){
 310                  ret = 1;
 311                  break;
 312              }
 313          }
 314      }while(ret == 1);
 315  };
 316

 317  /*----------------------------------------------------------------------*/
 318  void sort(struct spe_ctrl* sc){

sort�֐���SPE�ōs���\�[�e�B���O�̎��̂ł��D

 319      int i;
 320
 321      for(i=0; i<2; i++){
 322          put_status(sc->id, 0, i, sc);
 323      }

�܂��C�eSPE�͎����̏�Ԃ�0�ŏ��������܂��D

 324
 325      sort_phase1(sc);

sort_phase1�֐������s���܂��D
4�̋�Ԃɕ��������Ώۗv�f���eSPE�͏��Ԃɓǂݏo���ăL�[���v�Z���C
���C���������ɏ����߂��܂��D
���ɁC���̃L�[�����ɁC���ꂼ��̒S����Ԃ�O���C�㔼�ɕ����āC
���ꂼ����\�[�g(�N�C�b�N�\�[�g)���܂��D

 326      put_status(sc->id, 0, 1, sc);
 327      wait_spe(sc, 0, 1);

sort_phase1�̎��s�I����C�����̏��status[0]��1���Z�b�g���C
�S�Ă�SPE�̏��status[0]�� 1 �ɂȂ�܂ő҂��܂��D
�����܂łŁC�Ώ۔z���8�̋�Ԃɕ������Č����Ƃ��ɁC
���ꂼ��̕����z��̓\�[�g�ς݂ɂȂ��Ă��܂��D

 328
 329      if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
 330      else if(sc->id == 1) merge_sort(sc, 0, 3, 2);
 331      else if(sc->id == 2) merge_sort(sc, 0, 4, 5);
 332      else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
 333      put_status(sc->id, 1, 2, sc);
 334      wait_spe(sc, 1, 2);
 335

�eSPE���ƂɁC2���̋�Ԃ��}�[�W�\�[�g���܂��D
����329�s�ځ`334�s�ڂ̃X�e�b�v�́C����ȍ~6��J��Ԃ���܂��D
���̃A���S���Y���͊��S�V���b�t�����݌��������ɂ�����o�C�g�j�b�N�\�[�g
�ƌĂ΂�����\�[�e�B���O�A���S���Y���̈��ł�
(S�E�A�N�����u����\�[�e�B���O�E�A���S���Y���v�[�w�o��)�D
�����2^m�̃\�[�g�ςݕ����z����C2^(m-1)�̌v�Z���j�b�g�Ń\�[�g���邽��
�̃A���S���Y���ł��D
����͊ȒP�̂��߁C�eSPE���X�e�b�v���ƂɃ}�[�W�\�[�g�����Ԕԍ��𖾎�
���邽�߂ɓW�J���Ă���܂��D

�e�X��SPE���S���̋�Ԃ��\�[�g������C��Ԃ��X�V��(status[1]��2��)�C
����SPE�����̃X�e�b�v�̃\�[�g���I������܂ő҂��܂��D

 336      if(sc->id == 0)      merge_sort(sc, 1, 0, 2);
 337      else if(sc->id == 1) merge_sort(sc, 1, 1, 3);
 338      else if(sc->id == 2) merge_sort(sc, 1, 6, 4);
 339      else if(sc->id == 3) merge_sort(sc, 1, 7, 5);
 340      put_status(sc->id, 0, 3, sc);
 341      wait_spe(sc, 0, 3);

��2�X�e�b�v�ł��D

 342
 343      if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
 344      else if(sc->id == 1) merge_sort(sc, 0, 2, 3);
 345      else if(sc->id == 2) merge_sort(sc, 0, 5, 4);
 346      else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
 347      put_status(sc->id, 1, 4, sc);
 348      wait_spe(sc, 1, 4);

��3�X�e�b�v�ł��D

 349
 350      if(sc->id == 0)      merge_sort(sc, 1, 0, 4);
 351      else if(sc->id == 1) merge_sort(sc, 1, 1, 5);
 352      else if(sc->id == 2) merge_sort(sc, 1, 2, 6);
 353      else if(sc->id == 3) merge_sort(sc, 1, 3, 7);
 354      put_status(sc->id, 0, 5, sc);
 355      wait_spe(sc, 0, 5);

��4�X�e�b�v�ł��D

 356
 357      if(sc->id == 0)      merge_sort(sc, 0, 0, 2);
 358      else if(sc->id == 1) merge_sort(sc, 0, 1, 3);
 359      else if(sc->id == 2) merge_sort(sc, 0, 4, 6);
 360      else if(sc->id == 3) merge_sort(sc, 0, 5, 7);
 361      put_status(sc->id, 1, 6, sc);
 362      wait_spe(sc, 1, 6);

��5�X�e�b�v�ł��D

 363
 364      if(sc->id == 0)      merge_sort(sc, 1, 0, 1);
 365      else if(sc->id == 1) merge_sort(sc, 1, 2, 3);
 366      else if(sc->id == 2) merge_sort(sc, 1, 4, 5);
 367      else if(sc->id == 3) merge_sort(sc, 1, 6, 7);
 368      put_status(sc->id, 0, 7, sc);
 369      wait_spe(sc, 0, 7);

��6�X�e�b�v�ł��D
�����܂łŁC�Ώ۔z��̃\�[�e�B���O���I���������ƂɂȂ�܂��D

 370  };

sort�֐����s��́C���Ƃ��ė^����ꂽ�v�f�̔z��Ɠ����̈�ɁC
�\�[�g�ς݂̌��ʂ��i�[����Ă��܂��D

 371
 372  /*----------------------------------------------------------------------*/
 373  /* SPE main program                                                     */
 374  /*----------------------------------------------------------------------*/
 375  int main(int speid, unsigned long long argv)

SPE�̃��C���֐��ł��DPPE�̃v���O������
spe_wrap_resume_thread(swt_handle)���Ăяo�����ƁC���s���J�n����܂��D
�����́C�v���Z�X�ԍ�(speid : ���̃v���O�����ɂ�����SPE��ID�Ƃ͈قȂ�)
�ƁCPPE�v���O������ spe_wrap_create_dedicated_thread�֐��ŗ^����ꂽ��
��(�����sarg[i]�̃A�h���X)���^�����܂��D

 376  {
 377      dmaget((void*)&arg, argv, SPE_DMA_ALIGN);

�܂��C����argv(sarg[i]�̃A�h���X)�����C���������̃A�h���X�Ƃ��āC
dmaget���s���Csarg[i]�̓��e���\����arg(struct argumen�^)�ɓǂݍ��݂܂��D
�\����arg�ɂ́Csc_addr�Ƃ��������o�ϐ��ɁCctrl[i]�ւ̃|�C���^(struct
spe_ctrl�^)���������܂�Ă��܂��D

 378
 379      sc.flag = 0;
 380
 381      while(sc.flag==0){
 382          dmaget((void*)&sc, arg.sc_addr, SPE_DMA_ALIGN);
 383      }

�\����sc(struct spe_ctrl�^)�ɁC�\����arg�̃����o�ϐ�sc_addr�����C����
�����̃A�h���X�Ƃ��āCdmaget���s���Cctrl[i]��ǂݍ��݂܂��D
ctrl[i]�́Cdefine.h�̍\����spe_ctrl�Ő������������o�ϐ��������C
PPE�v���O������main.c�Cppe_sort�֐��Ń\�[�e�B���O�̂��߂ɕK�v�Ȓl����
�肳��Ă��܂��D

�\����sc�ɂ̓����o�ϐ�flag������Cflag��0�ȊO�̒l�ɂȂ�܂ł̓\�[�e�B
���O���J�n���܂���D
����́CSPE�X���b�h���s��C���΂炭�҂��Ȃ���spe_printf������ɓ��삵
�Ȃ��������Ƃ���ǉ����ꂽ�����ł��D
�܂��CSPE�v���O�����ŏ�����ǉ�����\�������邽�߁Cflag��p���ĊǗ�
���Ă��܂��D

 384
 385      /*** in this version, only four SPEs (SPE0 to SPE3) work hard! ***/
 386      if(4>sc.id) sort((struct spe_ctrl*)&sc);

�����SPE��4�����g���C�\�[�e�B���O���s�����߁C�\����sc�̃����o�ϐ�id
��0����3�܂ł̒l��^����ꂽSPE�������Csort�v���O���������s���܂��D
����ȊO��SPE�͉������s�����CSPE�v���O�������I�����܂��D

 387
 388      return 0;
 389  }
 390  /*----------------------------------------------------------------------*/

����ŁC�������Ńc�[���L�b�g�̉���͂����܂��ł��D
�����o�O���Ȃǂ� cellspeed2007@arch.cs.titech.ac.jp �܂ł��肢���܂��D

�}���`�R�A�v���O���~���O�R���e�X�g�uCell�X�s�[�h�`�������W2007�v���s�ψ���
http://www.hpcc.jp/sacsis/2007/cell-challenge/
<------------------------------------------------------------------------->
