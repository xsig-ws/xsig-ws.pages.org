<------------------------------------------------------------------------->
Cell Speed Challenge 2007
お試し版ツールキット Version 2007-01-08 の使い方と解説

【規定課題概要】
多数のデータの系列が入力として与えられたときに，これらをある順序にした
がって並び替える処理をソーティングと呼びます．Cellスピードチャレンジ2007
の規定課題は「ソーティング」です．
ソーティングでは，データに含まれる並べ替えのために参照される項目をキーと
呼びます．このキーは単精度の浮動小数点数(C言語におけるfloat型)とします．
また，キーの値が昇順(小さい値が前方，大きい値が後方になるような順序)に
なるようにデータを並び替えるものとします．
たとえば，高校などの学籍名簿のn個のデータ系列(A1, A2, ..., An)を，
学籍番号順に(学籍番号をキーとして)並び替える処理を考えます．
各データAiは，学籍番号の他に，その番号を持つ学生名や入学年月日などの情報
をもつかもしれません．
ソーティングはこれらのAiを学籍番号の小さい順に並び替えるという処理です．

【第１章 お試し版ツールキットの使い方】

Cellオンラインサービス celldev3.celltsb.com にログインしてください．
ホームディレクトリに speed というディレクトリを作成し，
そこにサンプルファイルコピーします．このためのコマンドは次の通りです．

$ cd ~
$ mkdir speed
$ cp /export/home/cell2007/share/toolkit02/* .

コンパイルおよび実行のためには，作成した speed というディレクトリに移動
して，make コマンドを利用します．このためのコマンドは次の通りです．

$ cd ~/speed
$ make
$ make run

２番目のmakeでコンパイル，３番目のmake runで実行を開始します．
32768個のデータのソーティングをおこない，その結果が正しい場合には，
[ppe_] * Result Good! *
といったメッセージと計算に要した時間を出力します．

【第２章 お試し版ツールキットのファイル構成】

作成したディレクトリには次のファイルがコピーされます．

(1) README.txt  : このファイル
(2) Code.txt    : すべてのソースコードをまとめたテキストファイル
(3) Makefile    : make コマンドが利用するファイル
(4) define.h    : すべてのプログラムがインクルードするファイル
(5) main.c      : PPEのためのメイン関数を含むCプログラム
(6) lib.c       : PPEのプログラムが利用するCの関数群
(7) spe1.c      : SPEのためのメイン関数を含むCプログラム

通常は，(1), (2), (3), (6) のファイルを変更する必要はありません．
(4), (5), (7) の３つのファイルを変更して，高速なソーティングプログラムの
実装を目指してください．

【第３章 お試し版ツールキットのプログラム解説】

ここからは，プログラムとMakefileの解説です．
数文字のスペースと数字（コードの行番号）から始まる行は，ソースコードの
一部です．それ以外の行が解説です．

(4) define.h    : すべてのプログラムがインクルードするファイル
(6) lib.c       : PPEのプログラムが利用する関数群
(5) main.c      : PPEのためのメイン関数を含むプログラム
(7) spe1.c      : SPEのためのメイン関数を含むプログラム
(3) Makefile    : make コマンドが利用するファイル
この順序でプログラムのコードをみていきます．

注意点．
このソースコードはお試し版ツールキットのものです．この版のツールキットは
プログラミングの導入として利用できるようにシンプルでさまざまな最適化の余地
のあるものを提供しています．コンテストで利用する正式なツールキットでは，
内容が大幅に変更される可能性があります．

file name: define.h
   1 /*-------------------------------------------------------------------*/
   2 /* Cell Speed Challenge 2007, Sample ToolKit Version 2007-01-08      */
   3 /*-------------------------------------------------------------------*/
   4 #define N 1024*32   /* the number of items, please modify this */
ここでソーティングのデータ数を指定します．ここでは，32kのデータを指定して
います．SPEのLocal Storeの作業領域として128KBを設定しているので，データ数
を32kより多く設定するとエラーとなる場合があります．まずは，32kより少ない
範囲で動作確認をしてください．
実際のコンテストでは大規模なデータを処理することになる予定です．

   5
   6 #define NUMBER_OF_SPES 1
利用するSPEの数を指定します．ここでは１を設定しています．
１～７の数字を設定します．

   7 #define EXE_SPE1 "speed/spe1"
   8 #define EXE_SPE2 "speed/spe1"
   9 #define EXE_SPE3 "speed/spe1"
  10 #define EXE_SPE4 "speed/spe1"
  11 #define EXE_SPE5 "speed/spe1"
  12 #define EXE_SPE6 "speed/spe1"
  13 #define EXE_SPE7 "speed/spe1"
それぞれのSPEが実行するプログラムのファイル名を指定します．ファイル名は
ユーザのディレクトリからのパスを指定します．
ここでは，すべてのSPEがspeed/spe1という名前のプログラムを利用する設定と
なっています．それぞれのSPEが異なるプログラムを実行してもかまいません．

  14
  15 #define SPE_BUF_SIZE (128*1024) /* allocate a 128KB buffer */
ここで指定したサイズのバッファをSPEに生成しています．
SPEのLocal Storeは256KBなので，その半分の128KBに設定しています．
この値は適切に調整してください．

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
この構造体は，PPEからSPEに情報を伝えるために利用します．
SPEでは，まずこの構造体のデータを受信して処理を進めます．これにより，
ソーティングのデータ数や，データが格納されているメモリのアドレスを知る
ことができます．

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
通常は，これら lib.c の内容を変更する必要はありません．
このツールキットでは，乱数により並び替えのキーを生成していますが，
コンテストでは乱数を利用するとは限りません．
また，このツールキットでは，データはキーのみから構成されるものとして
いますが，実際のデータはキーとその他の情報から構成されることに注意して
ください．

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
main.c は PPE が実行するプログラムです．
２つの配列を用意します．buf_in には，ソートすべき入力データが格納されます．
buf_outには，ソーティングされた結果を格納します．

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
メイン関数の最初の部分です．変数の定義と，SPEの標準出力のための初期化を
おこないます．

  28
  29     n = generate_list(buf_in, buf_out);
ソートすべき入力を取得します．データの数が返値として得られ，変数nに
格納します．

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
SPEのプログラム（スレッド）を生成します．利用するSPEの数は define.h の
NUMBER_OF_SPES で指定します．ここでは，すべてのSPEがEXE_SPE1で指定した
プログラムを実行するように記述されている点に注意してください．
それぞれが異なるプログラムを実行する場合には，この部分を修正する
必要があります．

  40
  41     time_start = my_clock();
  42     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_resume_thread(h[i]->id);
  43     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_wait_thread(h[i]->id);
  44     time_end = my_clock();
ソーティングの処理が始まる時の時刻を time_start に格納して，処理を開始
します．処理の終了をまって，すべてのSPEの処理が終わった時の時刻を
time_end に格納します．

  45
  46     for(i=0; i<NUMBER_OF_SPES; i++) spe_wrap_release_thread(h[i]);
SPEのスレッド（プログラム）を解放します．

  47
  48     check_list(buf_in, buf_out, N);
  49     printf("[ppe_] elapsed time(sec): %10.6f\n", time_end-time_start);
ソーティングの結果が正しいかどうかを確認します．正しくない場合には，
Wrong というメッセージが出力されます．続いて，経過時間を出力します．

  50
  51     return 0;
  52 }
  53 /*-------------------------------------------------------------------*/
ここまでがPPEのメイン関数です．すべてのSPEで同じプログラムを起動して，
SPEのみで処理をおこなう場合には，このファイルを変更する必要がありません．
一方，PPEでソーティングの一部を処理させる場合には適切な処理を追加して
ください．実際のコンテストでは，PPEによるソーティングの処理を禁止する
可能性がありますので，できるかぎりSPEで処理をおこなうようにしてください．

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
spe1.c はSPEで処理されるプログラムです．
128KBのバッファ（メモリ）を作業領域として確保しています．

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
DMA転送をおこなうmfc_get関数では，16KBを超えるデータの転送をおこなう
ことができません．
そこで，大きなデータをメインメモリからSPEのLocal Storeに転送するため
の関数 dma_get を実装しました．大きなデータの転送は，16KBの転送を繰り
返します．データ送受信と計算とのオーバラップや通信の高速化も重要です．

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
dma_getと同様に，大きなデータをLocal Storeからメインメモリに転送する
ための関数です．

  44 /*-------------------------------------------------------------------*/
  45 int main(int speid, unsigned long long argv)
  46 {
  47     dma_get((char *)&arg, argv, sizeof(struct argument));
  48     spe_printf("[spe%d] let's sort %d items\n", arg.id, arg.n);
SPEが実行するプログラムのメイン関数です．
まずは，構造体struct argumentで定義したデータをDMA転送で受け取ります．
この構造体に，ソートすべきデータの個数などが格納されます．
arg.id には，それぞれのSPEを識別するための番号(0～6)が格納されます．

  49
  50     if(arg.n<=SPE_BUF_ENTRY && arg.id==0){ /*** only spe0 works ***/
このプログラムは並列化が施されていません．すなわち，複数のSPEを利用した
としても１個のSPEのみがソーティングをおこないます．
複数のSPEを利用してソーティングをおこなうには，それぞれが異なる範囲を
部分的にソーティングをおこなうといった戦略による並列化が必要です．

  51         dma_get((char *)&buf, arg.adr_in, sizeof(float)*arg.n);
ソートすべきデータを取得します．
データはメインメモリの arg.adr_in が示すアドレスに格納されています．

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
ここがソーティングをおこなっている重要な部分です．ここでは挿入法による
ソーティングを実装しています．ただし，挿入法は実行時間が遅いので，
高速な並列ソーティングに変更する必要があります．

  66
  67         dma_put((char *)&buf, arg.adr_out, sizeof(float)*arg.n);
ソーティングの結果をメインメモリに格納します．
PPEのプログラムにおける buf_out のアドレスが，arg.adr_out に格納されて
いるので，ここが示すアドレスにソーティング済みの結果を転送します．

  68     }
  69     return 0;
  70 }
  71 /*-------------------------------------------------------------------*/
以上でソースコードの説明は終了です．

ここからは，ソースコードから実行ファイルを生成するための規則を記述した
Makefileです．makeコマンドが参照します．
ファイル構成やコンパイルの最適化オプションなどを変更しない場合には，
このMakefileを変更する必要はありません．
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
これらのファイルをspeedというディレクトリ以外に格納する場合には，上の
部分を変更します．また，実行のタイムアウトを15秒に設定していますので，
巨大な数のデータをソーティングする場合などはタイムアウトの時間を大きく
してください．

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
ここでコンパイルのための最適化オプションなどを設定します．通常は -O3 を
利用します．

  31
  32 #######################################################################
  33 main: main.c lib.c define.h Makefile
  34 	$(PPE_CC) $(PPE_ADD) main.c lib.c -o main $(PPE_LIB)
  35
  36 spe1: spe1.c define.h Makefile
  37 	$(SPE_CC) $(SPE_ADD) spe1.c -o spe1 $(SPE_LIB)
main.c 以外のファイルや，複数のファイルにプログラムを記述する場合などは，
この部分を変更します．

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

これで，お試し版ツールキットの解説はおしまいです．
質問やバグ情報などは cellspeed2007@arch.cs.titech.ac.jp までお願いします．

マルチコアプログラミングコンテスト「Cellスピードチャレンジ2007」実行委員会
http://www.hpcc.jp/sacsis/2007/cell-challenge/
<------------------------------------------------------------------------->
