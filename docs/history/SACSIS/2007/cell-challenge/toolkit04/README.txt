<------------------------------------------------------------------------->
Cell Speed Challenge 2007
お試し版ツールキット Version 2007-02-02 (Version 0.4)の使い方と解説

【概要】
このお試し版ツールキットは4個のSPEを利用した並列版のソーティングプログラム
です．Cell Speed Challenge 2007の規定課題部門の雛形としてご活用ください．
7個のSPEの利用やSIMDによる高速化など様々な最適化の余地が残されています．
このドキュメントに加えて，並列版ソーティングの実装に関するPowerPointの資料
を用意しました．こちらも参考にしてください．

【規定課題概要】
多数のデータの系列が入力として与えられたときに，これらをある順序にした
がって並び替える処理をソーティングと呼びます．Cellスピードチャレンジ2007
の規定課題は「ソーティング」です．
ソーティングでは，データに含まれる並べ替えのために参照される項目をキーと
呼びます．このキーは単精度の浮動小数点数(C言語におけるfloat型)とします．
また，キーの値が昇順(小さい値が前方，大きい値が後方になるような順序)に
なるようにデータを並び替えるものとします．

【ソーティングに関する補足】
これらの設定は，最終的な「規定課題 詳細およびツールキット」において変更に
なる可能性があります．
---------------------------------------------------------------------------
1. ソーティングの入力データ形式

多数のデータの系列が入力として与えられたときに，これらをある順序にしたがって
並び替える処理をソーティングと呼びます．
ソーティングの対象となる1つのデータを要素(element)と呼ぶことにします．
この要素には1つのキー(key)とある長さをもつリスト(list)が含まれます．
リストの長さをMという変数あるいは定数で表現すると，要素は，C言語の構造体を
用いて次の様に表現できます．

struct element{
    float key;
    float list[M];
};

説明を簡潔にするために構造体を利用して表現しましたが，実際には，入力データ
におけるリストの長さMは固定された値ではなく，プログラムが起動してから与え
られることになります．
---------------------------------------------------------------------------
2. 入力データおよび出力データの配置

プログラムの開始時点で，入力データは配列の先頭から格納されているものと
しています．
ソーティングが終了した時点で，出力データについても，ソーティングされた状態
で配列の先頭から格納することとしています．この時，配列の残りのメモリには
どのような値が格納されていてもかまいません．
---------------------------------------------------------------------------
3. リストの長さに関する制約

先に説明したように，ソーティングの要素には1つのキーとある長さをもつリスト
が含まれます．任意の長さのリストを考えると，データ転送の整列やサイズなどの
調整といった煩雑な処理が必要となります．このため，1つの要素のサイズが次式
になるように制約を加えることにします．

sizeof(struct element) = 2 ^ m (バイト)
m = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}

すなわち，1つの要素のサイズは16Bから16KBの範囲で，16Bの倍々となる値に制限
します．これにより，1つの要素の転送がDMAのサイズを超えることがなくなり，
プログラミングが容易になります．
---------------------------------------------------------------------------
4. リストからキーを得るための計算

入力データは要素の配列として与えらますが，それらのキーに対応する部分の値
はゼロで初期化されています．
ユーザはソーティングを開始する前に，リストからキーの値を生成しなければ
なりません．
リストからキーを計算する方式はプログラムの開始時に与えられます．
次の2つのうちのどちらかが指定されます．

(1)リストに含まれるすべての値の最大値
key = max{list[j]}, (j=0,1,2, ... , M-1)

(2)リストに含まれるすべての値の2乗の和
key = list[0]*list[0] + list[1]*list[1] +, ... , list[M-1] * list[M-1]

キー，リストの要素はともに単精度の浮動小数点数として表現されます．
また，それぞれの要素の先頭の4バイトにキーが格納されるものとし，残りに
リストが格納されるものとします．
---------------------------------------------------------------------------
5. SPEが受けとる情報

SPEのプログラムでは，次に示すような情報を受け取り利用することができます．

struct shared_control{
    unsigned int flag;
    unsigned int id;       // SPEの識別番号
    unsigned int spe_num;  // SPEの個数で，7に固定される．
    unsigned int buf_addr; // 配列の先頭アドレス
    unsigned int datanum;  // ソーティングすべき要素の数 N
    unsigned int listnum;  // リストの長さ M
    unsigned int keytype;  // リストからキーを計算する方式を指定する変数．
    unsigned int padding[25]; // 128バイトに整列するために利用している．
};
---------------------------------------------------------------------------

【第１章 お試し版ツールキットの使い方】

Cellオンラインサービス celldev3.celltsb.com にログインしてください．
ホームディレクトリに speed というディレクトリを作成し，
そこにサンプルファイルコピーします．このためのコマンドは次の通りです．

$ cd ~
$ mkdir speed
$ cp /export/home/cell2007/share/toolkit04/* .

コンパイルおよび実行のためには，作成した speed というディレクトリに移動
して，make コマンドを利用します．このためのコマンドは次の通りです．

$ cd ~/speed
$ make
$ make run

２番目のmakeでコンパイル，３番目のmake runで実行を開始します．

30MB(データ数983,040，リスト長7)のデータについて，
リスト内データの2乗の和をキーとしてソーティングを行います．

ソーティングの結果た正しい場合には，次のようなメッセージと計算に要した時間
を出力します．
[ppe_] * Result Good! *
[ppe_] n 983040, listnum 7, input data size 30 [MByte]
[ppe_] elapsed time(sec) 2.724527

結果が間違っている場合には Wrong! といったメッセージを表示します．

【第２章 お試し版ツールキットのファイル構成】

作成したディレクトリには次のファイルがコピーされます．

(1) README.txt  : このファイル
(2) Code.txt    : すべてのソースコードをまとめたテキストファイル
(3) Makefile    : make コマンドが利用するファイル
(4) define.h    : すべてのプログラムがインクルードするファイル
(5) main.c      : PPEのためのメイン関数を含むCプログラム
(6) lib.c       : PPEのプログラムが利用するCの関数群
(7) spe1.c      : SPEのためのメイン関数を含むCプログラム

通常は，(1), (2), (3), (4), (5), (6) のファイルを変更する必要はありません．
(7) spe1.c を変更して，高速なソーティングプログラムの実装を目指してください．

【第３章 お試し版ツールキットのプログラム解説】

(3) Makefile の内容は以前の版と同じです．
まずは，次のディレクトリの README.txt を読むことを勧めます．
/export/home/cell2007/share/toolkit02/

ここからは，プログラムの解説です．
数文字のスペースと数字（コードの行番号）から始まる行は，ソースコードの
一部です．それ以外の行が解説です．

(4) define.h    : すべてのプログラムがインクルードするファイル
(6) lib.c       : PPEのプログラムが利用する関数群
(5) main.c      : PPEのためのメイン関数を含むプログラム
(7) spe1.c      : SPEのためのメイン関数を含むプログラム
この順序でプログラムのコードをみていきます．

********************************************************************************
file name: define.h
   1  /*----------------------------------------------------------------------*/
   2  /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-02-02         */
   3  /*----------------------------------------------------------------------*/
   4  #define NUMBER_OF_SPES 7

利用するSPEの数を指定します．1～7の数字を設定します．
ここでは7を設定しています．

   5  #define EXE_SPE1 "speed/spe1"
   6  #define EXE_SPE2 "speed/spe1"
   7  #define EXE_SPE3 "speed/spe1"
   8  #define EXE_SPE4 "speed/spe1"
   9  #define EXE_SPE5 "speed/spe1"
  10  #define EXE_SPE6 "speed/spe1"
  11  #define EXE_SPE7 "speed/spe1"
  12  

各SPEで使用する実行ファイルが異なる場合には，ここでファイル名を指定します．
ただし，この版の実装では，複数のSPEを利用する場合にも全てのSPEが同一の
ファイルEXE_SPE1を実行するように記述されています．
異なる実行ファイルを指定するためには main.c の35行目付近の修正が必要です．

  13  #define KEY_TYPE 1 // choose 1 or 2
  14  #define LIST_NUM 7 // list size M
  15  #define INPUT_DATA (30 * 1024 * 1024) // input data size
  16  

キーの算出方法，リスト長，データサイズを指定します．
ここでは，KEY_TYPE = 1(リストの各データの2乗の和)，
リスト長は7，データサイズは30MBを指定しています．
実行時に頻繁にエラーが発生する場合には，データサイズを10MB程度に小さくして
試してみてください．

  17  #define ELEMENT_SIZE (4*(KEY_TYPE + LIST_NUM)) // element size
  18  #define DATA_NUM (INPUT_DATA/ELEMENT_SIZE)     // the number of element N
  19  #define USER_MEM (INPUT_DATA*3)                // user working memory

1つの要素のサイズ，および要素の数，利用可能なデータサイズを計算しています．

  20  
  21  /*----------------------------------------------------------------------*/
  22  #define DMA_TAG 0
  23  #define SPE_STDIO_BUF_SIZE 512
  24  #define SPE_DMA_ALIGN (128)

DMA転送用のタグ番号に名前(DMA_TAG)をつけています．
SPE_STDIO_BUF_SIZEで，SPEからの標準出力用のバッファサイズを指定してい
ます．
DMA転送の最小サイズである128バイトを，SPE_DMA_ALIGNという名前を付けて
います．

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

構造体argumentは，PPEからSPEに情報を伝えるために利用します．
構造体argumentはSPEごとにspe_ctrlを取得する際のアドレスだけが伝えられます．

構造体spe_ctrlでは，以下の情報をSPEがソーティングに関する情報を取得す
るために利用されます．
構造体spe_ctrlでは，以下の情報がPPEに伝えられます．
・ flag     : 計算開始フラグ(1になると計算が開始できる)
・ id       : SPE番号(0～7)
・ spe_num  : 利用可能なSPEの数(通常7) ##### 必要無いかも
・ buf_addr : ソーティング対象データが格納されたメモリの先頭アドレス
・ n        : 要素数
・ listnum  : リスト長
・ keytype  : キーの計算方法

SPEは，まずこの構造体argumentのデータを受信してソーティング情報が格納
されたアドレスを取得します．そのアドレスから構造体spe_ctrlのデータを受
信し，ソーティングに関する情報を得ます．
この際，flagの値をチェックしており，この値が0以外になると，ソーティン
グ情報が得られたことになり，処理が開始されます．

これらの構造体は，DMA転送を効率よく行うために，構造体のサイズが128バイ
トになるように，適切にパディングが設定されています．

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

チェックルーチンで使う構造体data_blockを宣言しています．

  17  
  18  /*----------------------------------------------------------------------*/
  19  double my_clock()
  20  {
  21      struct timeval tv;
  22      gettimeofday(&tv, NULL);
  23      return tv.tv_sec + (double)tv.tv_usec*1e-6;
  24  };

時間計測ルーチンmy_clockを宣言しています．
現在はgettimeofdayで時間を計測しています．

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

関数generate_listで乱数のソーティング対象データを作りポインタbufに格納します．
データはキー格納用にbuf[(listnum+1)*i]には0.0を格納し，有効なデータは入っ
ていません．

  41  
  42  /*----------------------------------------------------------------------*/
  43  float square_norm(float* n, int listnum){
  44      int i;
  45      float norm = 0.0;
  46      for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
  47      return norm;
  48  };

square_normはチェックルーチンで使用する，配列のポインタnからlistnum個
のfloatデータの2乗値の和を返す関数です．

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

ソーティングが完了していることを確認するcheck_list関数です．
構造体へのポインタssblockに，ソーティング配列の先頭を格納しておき，
float型の一次元配列を，構造体data_blockとして要素に簡易にアクセスでき
るようにします．
要素のキーの値を比較し，昇順に並んでいることを確認します．

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

lib.cより提供される関数です．
上から，
・my_clock()      : 時間計測ルーチン
・generate_list() : ソーティング対象データ生成ルーチン
・check_list()    : チェックルーチン

  18  
  19  volatile static float buf[USER_MEM/4] _ALIGN;
  20  volatile static struct argument sarg[NUMBER_OF_SPES] _ALIGN;
  21  volatile static struct spe_ctrl ctrl[NUMBER_OF_SPES] _ALIGN;
  22  double t1,t2; // start and end time stamp

main関数内で使う変数群です．
配列はそれぞれ_ALIGNで先頭が128バイト境界に整列されています．
・float buf[]          : ソーティング対象となるデータが格納される配列
・struct argument sarg : PPEからSPEに伝える情報が格納される構造体(define.h)
                         この構造体はSPEのmain関数の引数になります
・struct spe_ctrl ctrl : SPEが取得するソーティングに関する情報が格納される配列(define.h)
・double t1, t2        : 時間計測の始まりと終わりの値が格納される変数


  23  
  24  /*----------------------------------------------------------------------*/
  25  void ppe_sort(volatile float* buf, int n, int listnum, int keytype){

ppe_sort関数で，ソーティングを実行します．
まず，SPEスレッドの生成やアクセスヒントの設定，ソーティング情報の設定などを行います．
その後，SPEスレッドを実行し，SPEスレッドが終了するまで待ちます．

引数は，以下の4つです．
・volatile float* buf : ソーティング対象データが格納されたメモリのポインタ
・int n               : 要素数
・int listnum         : リスト長
・int keytype         : キーの計算方法，

  26     int i;
  27     swt_handle *h[NUMBER_OF_SPES];
  28     spere_object_id hint[NUMBER_OF_SPES][3];

スレッドハンドル(swt_handle) hおよびアクセスヒント(spere_object_id)
hintを宣言します．

  29  
  30     _spe_stdio_init(SPE_STDIO_BUF_SIZE);
  31     _spe_stdio_start();

SPEの標準出力を受け付けるサーバを起動します．
1回のspe_printfにつき，SPE_STDIO_BUF_SIZE(512Byte)で定義されたサイズ分，
標準出力を得ることができます．
もしも，spe_printfで出力する文字列がSPE_STDIO_BUF_SIZEを超えた場合，出
力が0になってしまいます．

  32  
  33     for(i=0; i<NUMBER_OF_SPES; i++){
  34         h[i] = spe_wrap_create_dedicated_thread(
  35             EXE_SPE1, (void*)&sarg[i], SIGNOTIFY_INIT_MODE);
  36         sarg[i].id      = i;
  37         sarg[i].sc_addr = (unsigned int)&ctrl[i];
  38     }

以下の処理では，SPEの数だけ，処理が実行されます．
スレッドハンドルhに，以下の条件でSPEスレッドを生成して設定します．
・SPE[i]で実行される実行バイナリとしてEXE_SPE1で指定されたファイルを指定
  (defie.h・絶対パス)
・引数にsarg[i]のアドレスを指定

SPEに与える引数sargに以下のメンバ変数を設定し，各SPEから参照できるようにします．
・int id               : SPE番号
・unsigned int sc_addr : それぞれのctrl(構造体spe_ctrl型)のアドレス

  39  
  40     for(i=0; i<NUMBER_OF_SPES; i++){
  41         ctrl[i].flag     = 0;
  42         ctrl[i].id       = i;
  43         ctrl[i].buf_addr = (unsigned int)buf;
  44         ctrl[i].n        = n;
  45         ctrl[i].listnum  = listnum;
  46         ctrl[i].spe_num  = NUMBER_OF_SPES;
  47     }

ソーティング情報spe_ctrlに，必要な値(define.hの構造体spe_ctrlの解説参
照)を設定します．

  48  
  49     for(i=0; i<NUMBER_OF_SPES; i++){
  50         hint[i][0] = spe_wrap_add_access_hint(
  51             h[i]->id, (void*)&sarg[i], SPE_DMA_ALIGN);
  52         hint[i][1] = spe_wrap_add_access_hint(
  53             h[i]->id, (void*)&ctrl[i], SPE_DMA_ALIGN);
  54         hint[i][2] = spe_wrap_add_access_hint(
  55             h[i]->id, (void*)buf, USER_MEM);
  56     }

struct argument sarg[7]，struct spe_control sarg[7], buf それぞれアク
セスヒントを設定します．

  57  
  58     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);

SPEスレッドを実行します．

  59  
  60     /**** start the sorting *****/
  61     t1 = my_clock();
  62     for(i=0; i<NUMBER_OF_SPES; i++) ctrl[i].flag = 1;

時間計測を開始し，ctrlのflagに1を格納し，SPEプログラムでソーティングの
処理を開始します(spe1.c，main関数参照)．

  63     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
  64     t2 = my_clock();
  65     /**** end   the sorting *****/

全てのSPEプログラムが終了するまで待ちます．
すなわち，PPE側ではSPEに実行を指示するのみで，ソーティングに関する処理
は行いません．
終了した後，時間計測を終了します．

  66  
  67     for(i=0; i<NUMBER_OF_SPES; i++){
  68         spe_wrap_remove_access_hint(h[i]->id, hint[i][0]);
  69         spe_wrap_remove_access_hint(h[i]->id, hint[i][1]);
  70         spe_wrap_remove_access_hint(h[i]->id, hint[i][2]);
  71         spe_wrap_release_thread(h[i]);
  72     }

アクセスヒントを解除し，スレッドを解放します．

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

SPEの標準出力を受け付けるサーバを停止します．
ppe_sort関数を終了します．

  87  
  88  /*----------------------------------------------------------------------*/
  89  int main(int argc, char *argv[])

PPEで実行されるmain関数です．

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

標準入力から，要素数，リスト長，キーの計算方法を取得します．
上記が設定されていない場合は，define.hからそれぞれの値を設定します．

 100  
 101     generate_list((float*)buf, n, listnum);
 102     ppe_sort((volatile float*)buf, n, listnum, keytype);
 103     check_list((float*)buf, n, listnum, keytype);

generate_listでbufに要素数n，リスト長listnumの対象データ配列を生成します．
ppe_sortでSPEスレッドを実行し，ソーティングを行います．
その結果をcheck_listで検証します．

 104  
 105     printf("[ppe_] n %d, listnum %d, input data size %d [MByte]\n",
 106            n, listnum, sizeof(float)*n*(listnum+1)/1024/1024);
 107     printf("[ppe_] elapsed time(sec) %8.6f\n", t2-t1);
 108     return 0;
 109  }
 110  /*----------------------------------------------------------------------*/

ソーティング対象データサイズなどを表示し，計測された計算時間を表示して
終了します．

********************************************************************************
SPEで実行されるプログラムについて解説します．
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

SPEプログラムで使用するヘッダファイルです．

  13  /*----------------------------------------------------------------------*/
  14  struct data_block{ // 32 Byte
  15      float key;
  16      float data[LIST_NUM];
  17  };

構造体data_blockです．キーと数値のリストからなります．これを使って，メ
インメモリからデータを取ってきたり，書き込んだりします．
現在のところは，リスト長LIST_NUMは7で固定なので，struct data_blockのサイズは
32Byteになります．

  18
  19  struct transfer_block{ // 128 Byte
  20      struct data_block dblock[4];
  21  };
  22

構造体transfer_blockです．Cell Broadband EngineはDMA転送の単位が128バ
イトごとなので，128バイトにstruct data_block(32バイト)を4つ並べます．

  23  struct sync_block{
  24      unsigned int status[32];
  25  };
  26

構造体sync_blockです．statusという名前のunsigned int型配列を持ち，これ
を使ってSPE間の同期を取ります．

  27  struct merge_return{
  28      int oa;
  29      int ob;
  30  };
  31  /*----------------------------------------------------------------------*/

構造体merge_returnです．マージソートに利用します．

  32
  33  volatile static struct argument arg _GALIGN;
  34  volatile static struct spe_ctrl sc _GALIGN;
  35  volatile static struct sync_block sb _GALIGN;
  36  volatile static struct transfer_block eb _GALIGN;
  37

各種グローバル変数を宣言しています．
全てのグローバル変数は_GALIGN属性を用い，128バイト境界に整列されています．
・arg : 引数として与えられる構造体です
・sc  : PPEからソーティングに関する情報を取得する構造体です
・sb  : SPE間の同期を取る際に利用する構造体です
・eb  : メインメモリとSPE上のローカルメモリの通信に利用する構造体です

  38  /*----------------------------------------------------------------------*/
  39  void dmaget(void* d, unsigned long long addr, unsigned int size){
  40      /***** DMA Data Transfer, Memory -> LS *****/
  41      mfc_get(d, addr, size, DMA_TAG, 0, 0);
  42      mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  43      mfc_write_tag_update_all();
  44      mfc_read_tag_status();
  45  }
  46

メインメモリからSPEのローカルストレージ(LS)へデータを転送するdmaget関
数です．引数addrで与えられたメインメモリのアドレスから，sizeバイトのデー
タをポインタdで示されたLSのメモリへ転送します．
addrで示されるアドレスは128バイト境界で，sizeは128バイトごとの転送が効
率が良いことが知られています．
dmaget関数はmfc_getというDMA転送用の関数を一回だけ呼ぶため，size
の最大値は16384バイト(16KB)に制限されます．

  47  /*----------------------------------------------------------------------*/
  48  void dmaput(void* d, unsigned long long addr, unsigned int size){
  49      /***** DMA Data Transfer, LS -> Memory *****/
  50      mfc_put(d, addr, size, DMA_TAG, 0, 0);
  51      mfc_write_tag_mask(DMA_TAG_MASK(DMA_TAG));
  52      mfc_write_tag_update_all();
  53      mfc_read_tag_status();
  54  }

SPEのLSからメインメモリへデータを転送するdmaput関
数です．
引数addrで与えられたメインメモリのアドレスへ，sizeバイトのデー
タをポインタdで示されたLSのメモリの内容を転送します．
dmaget関数と同様に，
addrで示されるアドレスは128バイト境界で，sizeは128バイトごとの転送が効
率が良いことが知られています．
dmaget関数はmfc_getというDMA転送用の関数を一回だけ呼ぶため，size
の最大値は16384バイト(16KB)に制限されます．

  55  /*----------------------------------------------------------------------*/
  56  float square_norm(float* n, int listnum){
  57      int i;
  58      float norm = 0.0;
  59      for(i=0; i<listnum; i++) norm += (n[i] * n[i]);
  60      return norm;
  61  };
  62

square_normはSPEプログラムで使用する，配列のポインタnからlistnum個
のfloatデータの2乗値の和を返す関数です．

  63  /*----------------------------------------------------------------------*/
  64  unsigned int get_eaddr(unsigned int saddr, int eid){
  65      unsigned int eaddr;
  66      eaddr = saddr + eid * 128;
  67      return eaddr;
  68  };
  69

get_eaddrは，128バイトを1ブロックとして，saddrで与えられるアドレスから
eidブロック目のアドレスを返す関数です．

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

get_tblockは，128バイトを1ブロックとして，saddrで与えられるアドレスから
eidブロック目から128バイトのデータを取得する関数です．
返値はstruct transfer_block型になります．

  80  /*----------------------------------------------------------------------*/
  81  void put_tblock(int eid, struct transfer_block* eblock, struct spe_ctrl* sc){
  82      unsigned int eaddr; // element address
  83      eaddr = get_eaddr(sc->buf_addr,eid);
  84      dmaput((void*)eblock, eaddr, sizeof(struct transfer_block));
  85  };

put_tblockは，saddrで与えられるメインメモリのアドレスに，128バイトを1
ブロックとしてeidブロック目に128バイトのデータを書き込む関数です．

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

get_dataは，id番目の要素をメインメモリから取り出す関数です．

DMA転送は128バイト単位であり，要素のサイズは32バイトであることから，1
つの要素を狙ってDMA転送することは困難です．そのため，128バイトを1単位
としてDMA転送を行い，要素はその後に取り出す方法を取ります．

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

swap_dbは，メインメモリ上のid1番目の要素と，id2番目の要素を交換する関
数です．
DMA転送は128バイト単位なので，それぞれの要素を含む128バイトのブロック
を読み込み，要素を交換して，書き戻す，という手順を取ります．
id1とid2の要素が同一の128バイトブロックに存在する場合の処理が，

 113      if(eid[0] == eid[1]){

で記述されています．

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

qs_sortは，引数で与えられたs番目の要素から，e番目の要素をクイックソー
トでソーティングする関数です．
要素にはあらかじめキーが計算され，格納されていることを前提とします．
sとeの中間の要素をピボットとして設定しています．

 171  /*----------------------------------------------------------------------*/
 172  void sort_phase1(struct spe_ctrl* sc){

sort_phase1は，各SPEがキーを計算し，部分配列のソーティングを行う関数です．

 173      int i,j;
 174      int elnum; // element number
 175      unsigned int eaddr;
 176      unsigned int s;
 177      unsigned int e;
 178
 179      elnum = sc->n / 16;

今回のプログラムでは，SPEを4つ使うので，対象の要素を4区間に分割し，
4つのSPEで，各区間の要素のキーを計算します．

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

182行目から189行目で，各要素のキーを計算します．
各区間では，128バイトの中に4つの要素が格納されているので，
キーの計算のためのDMA転送の回数は(要素数÷16)回になります．
要素を128バイトブロックごとに取得してキーを計算し，要素にキーを格納し
た後，同じ場所に書き戻しています．

 190
 191      // Sub sorting in a divided elements
 192      s = sc->id * elnum * 4;
 193      e = s + (elnum * 2);
 194      qs_sort(s,e-1,sc);
 195
 196      s = (sc->id * elnum * 4) + (elnum * 2);
 197      e = s + (elnum * 2);
 198      qs_sort(s,e-1,sc);

192行目から198行目の処理は，それぞれのSPEが担当する要素を前半，後半に
分割し，部分ソートを行っています．
つまり，ソーティング対象となる要素全体を8つの区間に分割し，各SPEが2つ
の区間をそれぞれ部分ソートすることになります．

 199  };
 200

 201  /*----------------------------------------------------------------------*/
 202  void merge_core(struct spe_ctrl* sc, struct merge_return* mr,
 203                  int sa, int sanum, int sb, int sbnum, int da, int danum){

merge_coreは，sa番目の要素およびsb番目の要素から，
それぞれsanum個，sbnum個の要素を取り出し，マージソートする関数です
(merge_sort_sub関数を参照)．
その際，struct merge_return* mrで指定されたオフセットoa，obが適用され
ます．
oa，obは言い換えるとsanum，sbnumのうち，ソートのために使用済みの要素数
を示していることになります．

結果は，da番目の区画からdanum個書き込まれます．
sanum = sbnum = danum であることを前提にしているため，2つの
入力列を全て処理する前に，出力列がいっぱいになってしまいます．

そのため，出力がいっぱいになったときには，sa,sbで読み出し中の要素番号を，
struct merge_return* mrに格納して返します．

 204      int i,j,oa,ob;
 205      struct data_block dba,dbb;
 206      struct transfer_block eblock;
 207      int d;
 208
 209      d  = da / 4;

書き込みは128バイトの転送ブロック単位で書き込みを行います．
読み出しは要素ごとに読み出しをするので，注意してください．

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

saまたはsbの要素配列が全て読み出し終わっていたら，他一方の
要素配列を出力列に読み込みます．

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

2つの要素配列から1つずつ要素を読み込み，キーを比較します．
キーが小さい方の要素を出力列に読み込みます．

 236
 237          j++;
 238          if(j == 4){
 239              put_tblock(d,&eblock,sc);

出力列に4つの要素が溜まるごとに，daの要素配列に書き出します．

 240              d++;
 241              i++;
 242              j = 0;
 243          }
 244      }
 245
 246      mr->oa = oa;
 247      mr->ob = ob;

sa,sbで読み出し中の要素番号を，struct merge_return* mrに格納して返します．

 248  }


 250  /*----------------------------------------------------------------------*/
 251  void merge_sort_sub(struct spe_ctrl* sc, int sa, int sanum,int sb,
 252                      int sbnum,int da, int danum,int db, int dbnum){

merge_sort_subは，sa番目の要素およびsb番目の要素から，
それぞれsanum個，sbnum個の要素を取り出し，マージソートする関数です．
結果はda番目の要素からdanum個，db番目の要素からdbnum個の要素に書き出さ
れます．

 253      struct merge_return mr;
 254
 255      mr.oa = 0;
 256      mr.ob = 0;
 257
 258      merge_core(sc, &mr, sa, sanum, sb, sbnum, da, danum);
 259      merge_core(sc, &mr, sa, sanum, sb, sbnum, db, dbnum);

struct merge_return mrを媒介として，da用とdb用に，2回のmerge_core関数を呼びます．

 260  };
 261

 262  /*----------------------------------------------------------------------*/
 263  struct sync_block get_status(int id, struct spe_ctrl* sc){
 264      dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 265      return sb;
 266  };
 267

get_statusはidで示されたSPE番号を持つSPEの状態を読み込む関数です．
各SPEの状態は，spe_ctrlで利用可能なメインメモリの終端から順に128バイト
単位で格納されています．
つまり，
idが 0 のSPEの状態は sc->buf_addr + USER_MEM - 128 から128バイト，
idが 1 のSPEの状態は sc->buf_addr + USER_MEM - 256 から128バイト，
というように格納されています．

 268  /*----------------------------------------------------------------------*/
 269  void put_status(int id, int sid, int status, struct spe_ctrl* sc){
 270      dmaget((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 271      sb.status[sid] = status;
 272      dmaput((void*)&sb, sc->buf_addr + USER_MEM - ((id + 1) * 128), 128);
 273  };
 274

put_statusはidで示されたSPE番号を持つSPEの状態を書き換える関数です．
基本的に，各SPEは自分のidの状態のみの書き換えを許すことで，状態の一貫
性を保持します．

 275  /*----------------------------------------------------------------------*/
 276  void merge_sort(struct spe_ctrl* sc, int direction, int isa, int isb){

merge_sortは，8つの区間に分割されたソーティング対象配列について，
isa番目の区間とisb番目の区間をマージソートする関数です．
マージソートであるため，対象配列と同じ大きさの一時保存領域へソート結
果を出力します．
また，マージソートは複数切り替えられるため，
問題格納領域のデータをマージソートして一時保存用のメモリ領域へ出力する，
一時保存領域のデータをマージソートして問題格納領域へ出力する，
という2種類の方向を指定するために，directionというフラグを利用します．
今回は，対象配列の末尾以降に出力するようにしています．

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

実際のマージソートのための引数を設定する．
問題格納領域のデータをマージソートして一時保存用のメモリ領域へ出力する場合．
isa番目の区画と，isb番目の区画をマージソートして，
(isa+8)番目の区画と(isb+8)番目の区画に結果を書き出す．

 287      }
 288      else{
 289          sa = elnum * (isa+8);
 290          sb = elnum * (isb+8);
 291          da = elnum * isa;
 292          db = elnum * isb;

一時保存用のメモリ領域から読み出したデータをマージソートして問題格納領域へ出力する場合．
(isa+8)番目の区画と(isb+8)番目の区画をマージソートして，
isa番目の区画と，isb番目の区画に結果を書き出す．

 293      }
 294
 295      merge_sort_sub(sc, sa, elnum, sb, elnum, da, elnum, db, elnum);

設定された引数を用いてマージソートを行う．

 296  };

 298  /*----------------------------------------------------------------------*/
 299  void wait_spe(struct spe_ctrl* sc, int sid, int status){

wait_speは各SPEの同期を取るための関数です．
関数が実行されると，4つのSPEの状態を順番に読み出し，
全てがstatusで与えられたになるまで待ちます．

wait_spe関数を使って，SPE間の同期を取ることになります．

sidはstruct sync_blockのメンバであるstatus[32]の何番目を
同期のための状態として参照するかを指定します．

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

sort関数はSPEで行うソーティングの実体です．

 319      int i;
 320
 321      for(i=0; i<2; i++){
 322          put_status(sc->id, 0, i, sc);
 323      }

まず，各SPEは自分の状態を0で初期化します．

 324
 325      sort_phase1(sc);

sort_phase1関数を実行します．
4つの区間に分割した対象要素を各SPEは順番に読み出してキーを計算し，
メインメモリに書き戻します．
次に，そのキーを元に，それぞれの担当区間を前半，後半に分けて，
それぞれをソート(クイックソート)します．

 326      put_status(sc->id, 0, 1, sc);
 327      wait_spe(sc, 0, 1);

sort_phase1の実行終了後，自分の状態status[0]に1をセットし，
全てのSPEの状態status[0]が 1 になるまで待ちます．
ここまでで，対象配列を8つの区間に分割して見たときに，
それぞれの部分配列はソート済みになっています．

 328
 329      if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
 330      else if(sc->id == 1) merge_sort(sc, 0, 3, 2);
 331      else if(sc->id == 2) merge_sort(sc, 0, 4, 5);
 332      else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
 333      put_status(sc->id, 1, 2, sc);
 334      wait_spe(sc, 1, 2);
 335

各SPEごとに，2つずつの区間をマージソートします．
この329行目～334行目のステップは，これ以降6回繰り返されます．
このアルゴリズムは完全シャッフル相互結合方式におけるバイトニックソート
と呼ばれる並列ソーティングアルゴリズムの一種です
(S・アクル著「並列ソーティング・アルゴリズム」啓学出版)．
これは2^m個のソート済み部分配列を，2^(m-1)個の計算ユニットでソートするため
のアルゴリズムです．
今回は簡単のため，各SPEがステップごとにマージソートする区間番号を明示
するために展開してあります．

各々のSPEが担当の区間をソートした後，状態を更新し(status[1]を2に)，
他のSPEがこのステップのソートが終了するまで待ちます．

 336      if(sc->id == 0)      merge_sort(sc, 1, 0, 2);
 337      else if(sc->id == 1) merge_sort(sc, 1, 1, 3);
 338      else if(sc->id == 2) merge_sort(sc, 1, 6, 4);
 339      else if(sc->id == 3) merge_sort(sc, 1, 7, 5);
 340      put_status(sc->id, 0, 3, sc);
 341      wait_spe(sc, 0, 3);

第2ステップです．

 342
 343      if(sc->id == 0)      merge_sort(sc, 0, 0, 1);
 344      else if(sc->id == 1) merge_sort(sc, 0, 2, 3);
 345      else if(sc->id == 2) merge_sort(sc, 0, 5, 4);
 346      else if(sc->id == 3) merge_sort(sc, 0, 7, 6);
 347      put_status(sc->id, 1, 4, sc);
 348      wait_spe(sc, 1, 4);

第3ステップです．

 349
 350      if(sc->id == 0)      merge_sort(sc, 1, 0, 4);
 351      else if(sc->id == 1) merge_sort(sc, 1, 1, 5);
 352      else if(sc->id == 2) merge_sort(sc, 1, 2, 6);
 353      else if(sc->id == 3) merge_sort(sc, 1, 3, 7);
 354      put_status(sc->id, 0, 5, sc);
 355      wait_spe(sc, 0, 5);

第4ステップです．

 356
 357      if(sc->id == 0)      merge_sort(sc, 0, 0, 2);
 358      else if(sc->id == 1) merge_sort(sc, 0, 1, 3);
 359      else if(sc->id == 2) merge_sort(sc, 0, 4, 6);
 360      else if(sc->id == 3) merge_sort(sc, 0, 5, 7);
 361      put_status(sc->id, 1, 6, sc);
 362      wait_spe(sc, 1, 6);

第5ステップです．

 363
 364      if(sc->id == 0)      merge_sort(sc, 1, 0, 1);
 365      else if(sc->id == 1) merge_sort(sc, 1, 2, 3);
 366      else if(sc->id == 2) merge_sort(sc, 1, 4, 5);
 367      else if(sc->id == 3) merge_sort(sc, 1, 6, 7);
 368      put_status(sc->id, 0, 7, sc);
 369      wait_spe(sc, 0, 7);

第6ステップです．
ここまでで，対象配列のソーティングが終了したことになります．

 370  };

sort関数実行後は，問題として与えられた要素の配列と同じ領域に，
ソート済みの結果が格納されています．

 371
 372  /*----------------------------------------------------------------------*/
 373  /* SPE main program                                                     */
 374  /*----------------------------------------------------------------------*/
 375  int main(int speid, unsigned long long argv)

SPEのメイン関数です．PPEのプログラムで
spe_wrap_resume_thread(swt_handle)が呼び出されると，実行が開始されます．
引数は，プロセス番号(speid : このプログラムにおけるSPEのIDとは異なる)
と，PPEプログラムの spe_wrap_create_dedicated_thread関数で与えられた引
数(今回はsarg[i]のアドレス)が与えられます．

 376  {
 377      dmaget((void*)&arg, argv, SPE_DMA_ALIGN);

まず，引数argv(sarg[i]のアドレス)をメインメモリのアドレスとして，
dmagetを行い，sarg[i]の内容を構造体arg(struct argumen型)に読み込みます．
構造体argには，sc_addrというメンバ変数に，ctrl[i]へのポインタ(struct
spe_ctrl型)が書き込まれています．

 378
 379      sc.flag = 0;
 380
 381      while(sc.flag==0){
 382          dmaget((void*)&sc, arg.sc_addr, SPE_DMA_ALIGN);
 383      }

構造体sc(struct spe_ctrl型)に，構造体argのメンバ変数sc_addrをメインメ
モリのアドレスとして，dmagetを行い，ctrl[i]を読み込みます．
ctrl[i]は，define.hの構造体spe_ctrlで説明したメンバ変数を持ち，
PPEプログラムのmain.c，ppe_sort関数でソーティングのために必要な値が設
定されています．

構造体scにはメンバ変数flagがあり，flagが0以外の値になるまではソーティ
ングを開始しません．
これは，SPEスレッド実行後，しばらく待たないとspe_printfが正常に動作し
なかったことから追加された処理です．
また，SPEプログラムで処理を追加する可能性があるため，flagを用いて管理
しています．

 384
 385      /*** in this version, only four SPEs (SPE0 to SPE3) work hard! ***/
 386      if(4>sc.id) sort((struct spe_ctrl*)&sc);

今回はSPEを4つだけ使い，ソーティングを行うため，構造体scのメンバ変数id
が0から3までの値を与えられたSPEだけが，sortプログラムを実行します．
それ以外のSPEは何も実行せず，SPEプログラムを終了します．

 387
 388      return 0;
 389  }
 390  /*----------------------------------------------------------------------*/

これで，お試し版ツールキットの解説はおしまいです．
質問やバグ情報などは cellspeed2007@arch.cs.titech.ac.jp までお願いします．

マルチコアプログラミングコンテスト「Cellスピードチャレンジ2007」実行委員会
http://www.hpcc.jp/sacsis/2007/cell-challenge/
<------------------------------------------------------------------------->
