#ifndef _GRICHALLE_API_H_
#define _GRICHALLE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
get_problem()とanswer_problem()は同じプロセスで呼ぶこと
途中でexitしてはいけない
*/

int get_problem(const int problem_number,
		const char* config_dir,
		const char** key_string,
		int* width, int* height);
/* problem_number : 問題番号
   config_dirはお試し版で正答ファイルの置いてあるディレクトリを指定する
   予選・決勝ではNULLとすること 
   key_string はstaticに取られたconst char*を返す

   return value : 0 / 正常
		: not 0 / 異常
*/

int get_problem_files(const int problem_number, const char* key_string,
		      const char* problem_dir,
		      const char** problem_files);
/* key_string はstart_contestから得たもの
   problem_files にはローカルにアクセス可能なファイル名(フルパス)が返される
   各ファイル名の直後にはデリミタとして : (コロン)がおいている
   problem_dirはお試し版で問題ファイルの置いてあるディレクトリを指定する
   予選・決勝ではNULLとすること 

   return value : 0 / 正常
		: not 0 / 異常
*/

#define GRCH_CORRECT	0
#define GRCH_OK		1
#define GRCH_WRONG	2
#define GRCH_NG		3

int answer_problem(const int problem_number, const char* key_string,
                   const int result,
		   const char* file_coordinates,
		   const char* config_dir);
/* resultは解答の建物の数
   look_coordinatesには解答に使った全ての画像の座標情報を指定
   (0, 0), (1, 0), (2, 0),...,(1, 0), (1, 1),...
   の順で、その座標のファイルを使用していれば'1'、
   使用していなければ'0'を書いた文字列を与える
   config_dirはお試し版での正答ファイルの置いてあるディレクトリを指定する
   予選・決勝ではNULLとすること 

   return value : GRCH_CORRECT / 正解
		: GRCH_OK      / 使用したproblem_filesに関しては正解
		: GRCH_WRONG   / 不正解
		: GRCH_NG      / get_problemと同じノードで呼んでいない等、
		                 不正な呼び出し
   標準出力に解答時間を可読形式にしたものと、正解証明の鍵が出力される
   正解証明の鍵はCORRECTの場合にのみ付加
*/

#ifdef __cplusplus
};
#endif

#endif /* _GRICHALLE_API_H_ */
