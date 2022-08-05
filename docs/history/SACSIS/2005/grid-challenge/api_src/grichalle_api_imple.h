#ifndef _GRICHALLE_API_IMPLE_H_
#define _GRICHALLE_API_IMPLE_H_

#include <sys/time.h>
#include <stdio.h>

#define GRCH_FNAME_MAX_LEN 256
#define GRCH_LOCAL_FILE_MAX_N 20000

#define BOOL int
#define TRUE 1
#define FALSE 0

struct Challenge_data
{
  int prob_num;
  int w;
  int h;
  struct timeval start_time;
  time_t base_time;
};

enum { P_INVALID, P_OTAMESHI, P_YOSEN, P_HONSEN };
static int problem_number_check(const int pnum);

static BOOL answer_fname(const char* config_dir, const int pnum,
			 char* ans_fname);

struct part_ans
{
  int total;
  int left;
  int upper;
};
static void set_part_ans(struct part_ans *pa, int t, int l, int u);
static int coord_a(int x, int y, int width);

struct Answer_file
{
  FILE *fp;
  BOOL is_encripted;
  int prob_num;
  int w;
  int h;
};

static BOOL af_open(struct Answer_file *af, const char* fname);
static void af_close(struct Answer_file *af);
static BOOL af_read_header(struct Answer_file *af);
static BOOL af_read_body(struct Answer_file *af, struct part_ans ans[]);

static void grch_out(char* format, ...);
static int cut_and_atoi(const char* str, int len);
static void readable_enc(const unsigned char* str, int len, char* retbuf); // retbufはstrの2倍の長さ + 1 (for NULL terminate)
static void readable_dec(const char* str, unsigned char* retbuf); // retbuf is NOT NULL teminated

static void prepare_global_data();
#define PROBLEM_KEY_LEN 42
// problem_key: prob_num(2bytes), starttime - basetime(8bytes), md5(32bytes)
static void make_problem_key(int prob_num, char *retbuf); // global_dataのstart_timeを読む
static BOOL check_problem_key(const char *problem_key);
static void make_secret_md5(const char *data, int data_len,
			    unsigned char *md5_str);
// md5_str は MD5_DIGEST_LENGTH + 1 の長さを確保すること
static const char *file_encoding_key(int prob_num);

static const char *get_problem_dir(int prob_num, const char *user_problem_dir);
// お試しならそのままuser_problem_dirを、予選なら/home/endoを返す
static BOOL lock_fd(int fd);
static BOOL unlock_fd(int fd);
static BOOL logging_honsen(const char* buf);

#endif /* _GRICHALLE_API_IMPLE_H_ */
