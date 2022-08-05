#include "grid_challenge_api.h"
#include "grichalle_api_imple.h"

#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>
#include <openssl/md5.h>
#include <openssl/blowfish.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct Challenge_data challenge_data;

void
set_part_ans(struct part_ans *pa, int t, int l, int u)
{
  pa->total = t; pa->left = l; pa->upper = u;
}

inline int coord_a(int x, int y, int width)
{
  return y * width + x;
}

int
problem_number_check(const int pnum)
{
  if (0 < pnum && pnum <= 10) return P_OTAMESHI;
  else if (11 <= pnum && pnum <= 20) return P_YOSEN;
  else if (21 <= pnum && pnum <= 26) return P_HONSEN;
  return P_INVALID;
}

const char *
get_problem_dir(int prob_num, const char *user_problem_dir)
{
  DIR *test_dp;
  const char *test_dir = NULL;
  static char ret_dir[100];
  int stage = problem_number_check(prob_num);
  if (stage == P_YOSEN) { // 予選
    test_dir = "/home/endo/yd/xet/";
    if ((test_dp = opendir(test_dir)) == NULL) {
      test_dir = "/usr/users/endo/yd/xet/";
      if ((test_dp = opendir(test_dir)) == NULL) return NULL;
    }
    closedir(test_dp);
    snprintf(ret_dir, 100, "%s%02d", test_dir, prob_num);
    return ret_dir;
  } else if (stage == P_HONSEN) { // 本選
    test_dir = "/home/endo/kd/avaj/";
    if ((test_dp = opendir(test_dir)) == NULL) {
      test_dir = "/usr/users/endo/kd/avaj/";
      if ((test_dp = opendir(test_dir)) == NULL) return NULL;
    }
    closedir(test_dp);
    snprintf(ret_dir, 100, "%s%02d", test_dir, prob_num);
    return ret_dir;
  } else {
    return user_problem_dir;
  }
}

BOOL
lock_fd(int fd)
{
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = (off_t) 0;
  lock.l_len = (off_t) 0;

  if (fcntl(fd, F_SETLKW, &lock) < 0) return FALSE;
  return TRUE;
}

BOOL
unlock_fd(int fd)
{
  struct flock lock;
  lock.l_type = F_UNLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = (off_t) 0;
  lock.l_len = (off_t) 0;

  if (fcntl(fd, F_SETLKW, &lock) < 0) return FALSE;
  return TRUE;
}

BOOL
logging_honsen(const char* buf)
{
  DIR *test_dp;
  const char *test_dir = NULL;
  char logfname[256];
  int log_fd;
  test_dir = "/home/endo/kd/loglog";
  if ((test_dp = opendir(test_dir)) == NULL) {
    test_dir = "/usr/users/endo/kd/loglog";
    if ((test_dp = opendir(test_dir)) == NULL) return FALSE;
  }
  snprintf(logfname, 256, "%s/finalstage.log", test_dir);
  umask(0000);
  if ((log_fd = open(logfname, O_RDWR | O_CREAT,
		     S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
       ) == -1) return FALSE;
  if (!lock_fd(log_fd)) { close(log_fd); return FALSE; }
  if (lseek(log_fd, 0L, SEEK_END) == -1) { close(log_fd); return FALSE; }
  write(log_fd, buf, strlen(buf));
  if (fsync(log_fd) != 0) { close(log_fd); return FALSE; }
  if (!unlock_fd(log_fd)) { close(log_fd); return FALSE; }
  close(log_fd);
  return TRUE;
}

int
get_problem(const int problem_number,
	    const char* config_dir_org,
	    const char** key_string,
	    int* width, int* height)
{
  char config_str[GRCH_FNAME_MAX_LEN];
  struct Answer_file ans_f;
  char prob_key_str[PROBLEM_KEY_LEN + 1];
  static char key_buf[100];
  const char *config_dir = NULL;
  int stage = P_INVALID;

  prepare_global_data();

  if ((stage = problem_number_check(problem_number)) == P_INVALID) {
    grch_out("problem_number is wrong"); return 1;
  }

  //XXX todo: check otameshi, yosen, kessyou

  config_dir = get_problem_dir(problem_number, config_dir_org);
  if (!answer_fname(config_dir, problem_number, config_str)) {
    grch_out("config_dir is wrong"); return 1;
  }
  if (!af_open(&ans_f, config_str)) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot find config file in %s", config_dir); return 1;
    } else {
      grch_out("Internal error. cannot find config file"); return 1;
    }
  }
  if (!af_read_header(&ans_f)) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot read config file in %s", config_dir); return 1;
    } else {
      grch_out("Internal error. cannot read config file"); return 1;
    }
  }
  *width = ans_f.w; *height = ans_f.h;
  challenge_data.prob_num = ans_f.prob_num;
  challenge_data.w = ans_f.w;
  challenge_data.h = ans_f.h;
  af_close(&ans_f);


  /*
  BF_KEY bf_key;
  unsigned char key_str[] = "otameshiOTAMESHI";
  BF_set_key(&bf_key, 16, key_str);
  unsigned char enc_buf[8];
  BF_ecb_encrypt((unsigned char *)("enctest0"), enc_buf, &bf_key, BF_ENCRYPT);
  char enc_readable_buf[17];
  readable_enc(enc_buf, 8, enc_readable_buf);
  */

  
  if (stage == P_HONSEN) {
    char buf[256];
    char datebuf[100];
    uid_t uid = getuid();
    char *env_user;
    struct timeval log_time;
    if (gettimeofday(&log_time, NULL) != 0) {
      grch_out("gettimeofday error"); return 1;
    }
    strftime(datebuf, 100, "%b %d %H:%M:%S", localtime(&(log_time.tv_sec)));
    env_user = getenv("USER");
    if (env_user == NULL) { env_user = ""; }
    snprintf(buf, 256, "%s, %s(%d), problem %d start\n", datebuf, env_user,
	     uid, problem_number);
    if (!logging_honsen(buf)) {
      grch_out("Internal error"); return 1;
    }
  }

  if (gettimeofday(&(challenge_data.start_time), NULL) != 0) {
    grch_out("gettimeofday error"); return 1;
  }
  make_problem_key(problem_number, prob_key_str);
  
  snprintf(key_buf, 100, "%s-%s",
	   file_encoding_key(problem_number), prob_key_str);
  *key_string = key_buf;
  
  grch_out("Problem %2d: Started", problem_number);
  return 0;
}

int
get_problem_files(const int problem_number, const char* key_string,
		  const char* problem_dir_org,
		  const char** problem_files)
{
  char prob_dir[GRCH_FNAME_MAX_LEN];
  int prob_dir_size;
  DIR *dp;
  struct dirent *dir;
  static char rbuf[GRCH_FNAME_MAX_LEN * GRCH_LOCAL_FILE_MAX_N];
  char *p;
  const char *problem_dir = NULL;
  int stage = P_INVALID;

  if ((stage = problem_number_check(problem_number)) == P_INVALID) {
    grch_out("problem_number is wrong"); return 1;
  }
  //XXX todo: check otameshi, yosen, kessyou

  // check key_string
  if (!check_problem_key(key_string)) {
    grch_out("key_string is wrong"); return 1;
  }
  problem_dir = get_problem_dir(problem_number, problem_dir_org);
  if (problem_dir == NULL || problem_dir[0] == '\0') {
    grch_out("problem_dir is wrong"); return 1;
  }
  strncpy(prob_dir, problem_dir, GRCH_FNAME_MAX_LEN - 1);
  prob_dir[GRCH_FNAME_MAX_LEN - 1] = '\0';

  prob_dir_size = strlen(prob_dir);
  if (prob_dir[prob_dir_size - 1] != '/') {
    if (prob_dir_size >= GRCH_FNAME_MAX_LEN - 1) {
      grch_out("problem_dir is too long"); return 1;
    }
    strcat(prob_dir, "/");
    prob_dir_size++;
  }

  if ((dp = opendir(problem_dir)) == NULL) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot open problem_dir :%s", problem_dir); return 1;
    } else {
      grch_out("cannot open problem_dir"); return 1;
    }
  }

  p = rbuf;
  *p = '\0';
  while ((dir = readdir(dp)) != NULL) {
    char fname[GRCH_FNAME_MAX_LEN];
    char *extp;
    if (dir->d_name[0] == '.') continue; // "."や".."を避ける
    strncpy(fname, dir->d_name, GRCH_FNAME_MAX_LEN - 1);
    fname[GRCH_FNAME_MAX_LEN - 1] = '\0';
    extp = strrchr(fname, '.');
    if (extp == NULL) continue;
    extp++;
    if (!((strncmp(extp, "mdt", 3) == 0) || (strncmp(extp, "rdt", 3) == 0)))
      continue;
    strcat(p, prob_dir);
    strcat(p, fname);
    strcat(p, ":");
    p += prob_dir_size + strlen(fname) + 1;
  }
  *problem_files = rbuf;

  return 0;
}

int
answer_problem(const int problem_number, const char* key_string,
		const int result,
		const char* file_coordinates,
		const char* config_dir_org)
{
  struct timeval end_time;
  int w_x;
  int w_y;
  int i;
  char prob_key_str[PROBLEM_KEY_LEN + 1];
  char correct_key_buf[100];
  char config_str[GRCH_FNAME_MAX_LEN];
  struct Answer_file ans_f;
  struct part_ans *ans;
  int correct_ans;
  BOOL lack;
  int x, y;
  int msec;
  int retval;
  static char *retstr[] = {"correct.",
			   "correct, but you did not count all files. To be qualified, you should count all.",
			   "wrong."};
  const char *config_dir = NULL;
  int stage = P_INVALID;
  
  if (gettimeofday(&end_time, NULL) != 0) {
    grch_out("gettimeofday error"); return GRCH_NG;
  }

  if ((stage = problem_number_check(problem_number)) == P_INVALID) {
    grch_out("problem_number is wrong"); return GRCH_NG;
  }
  if (stage == P_HONSEN) {
    char buf[256];
    char datebuf[100];
    uid_t uid = getuid();
    char *env_user;
    strftime(datebuf, 100,
	     "%b %d %H:%M:%S",
	     localtime(&(end_time.tv_sec)));
    env_user = getenv("USER");
    if (env_user == NULL) { env_user = ""; }
    snprintf(buf, 256, "%s, %s(%d), problem %d finish\n", datebuf, env_user,
	     uid, problem_number);
    if (!logging_honsen(buf)) {
      grch_out("Internal error"); return 1;
    }
  }
  if (challenge_data.prob_num != problem_number) {
    grch_out("problem_number changed"); return GRCH_NG;
  }

  w_x = challenge_data.w;
  w_y = challenge_data.h;
  // file_coordinatesがそろっているかチェック
  for (i = 0; i < w_x * w_y; i++) {
    if (!(file_coordinates[i] == '0' || file_coordinates[i] == '1')) {
      grch_out("file_coordinates is wrong"); return GRCH_NG;
    }
  }

  // key のチェック
  make_problem_key(problem_number, prob_key_str);
  snprintf(correct_key_buf, 100, "%s-%s",
	   file_encoding_key(problem_number), prob_key_str);
  if (strcmp(correct_key_buf, key_string) != 0) {
    grch_out("key_string changed"); return GRCH_NG;
  }
  
  config_dir = get_problem_dir(problem_number, config_dir_org);
  if (!answer_fname(config_dir, problem_number, config_str)) {
    grch_out("problem_number changed"); return GRCH_NG;
  }
  if (!af_open(&ans_f, config_str)) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot find config file in %s", config_dir); return GRCH_NG;
    } else {
      grch_out("Internal error. cannot find config file"); return GRCH_NG;
    }
  }
  if (!af_read_header(&ans_f)) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot read config file in %s", config_dir); return GRCH_NG;
    } else {
      grch_out("Internal error. cannot read config file"); return GRCH_NG;
    }
  }
  if (problem_number != ans_f.prob_num ||
      w_x != ans_f.w || w_y != ans_f.h) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot read config file in %s", config_dir); return GRCH_NG;
    } else {
      grch_out("Internal error. cannot read config file"); return GRCH_NG;
    }
  }
  ans = (struct part_ans *)calloc(w_x * w_y, sizeof(struct part_ans));
  if (!af_read_body(&ans_f, ans)) {
    if (stage == P_OTAMESHI) {
      grch_out("cannot read config file in %s", config_dir); return GRCH_NG;
    } else {
      grch_out("Internal error. cannot read config file"); return GRCH_NG;
    }
  }
  af_close(&ans_f);

  correct_ans = 0;
  lack = FALSE;
  for (x = 0; x < w_x; x++) {
    for (y = 0; y < w_y; y++) {
      int ca = coord_a(x, y, w_x);
      if (file_coordinates[ca] == '0') {
	lack = TRUE; continue;
      }
      correct_ans += ans[ca].total;
      if (x != 0 && file_coordinates[coord_a(x - 1, y, w_x)] != '0')
	correct_ans -= ans[ca].left;
      if (y != 0 && file_coordinates[coord_a(x, y - 1, w_x)] != '0')
	correct_ans -= ans[ca].upper;
    }
  }

  free(ans);
  
  msec = (end_time.tv_sec - challenge_data.start_time.tv_sec) * 1000 +
    ((int)(end_time.tv_usec - challenge_data.start_time.tv_usec) / 1000);
  grch_out("Problem %d: Finished", problem_number);
  grch_out("Elapsed time is %d.%02dsec", msec / 1000, (msec / 10) % 100);
  if (correct_ans == result) {
    if (lack) {
      retval = GRCH_OK;
    } else {
      retval = GRCH_CORRECT;
    }
  } else {
    retval = GRCH_WRONG;
  }
  grch_out("Your answer is %s", retstr[retval]);
  if (retval == GRCH_CORRECT) {
  //  if (1) {
    char buf[256];
    char datebuf[100];
    unsigned char md5_str[MD5_DIGEST_LENGTH + 1];
    char md5_readable_buf[MD5_DIGEST_LENGTH * 2 + 1];
    strftime(datebuf, 100,
	     "%a %b %d %H:%M:%S %Y",
	     localtime(&(challenge_data.start_time.tv_sec)));
    snprintf(buf, 256, "Correct, prob %02d, %d.%02dsec, at %s.",
	     problem_number, msec / 1000, (msec / 10) % 100,
	     datebuf);
    make_secret_md5(buf, strlen(buf), md5_str);
    readable_enc(md5_str, MD5_DIGEST_LENGTH, md5_readable_buf);
    grch_out("<<%s / key = %s>>", buf, md5_readable_buf);
  }

  /*
  string key_s(key_string);
  int p = key_s.find('-');
  string enc_s(key_s.substr(p + 1));
  BF_KEY bf_key;
  unsigned char key_str[] = "otameshiOTAMESHI";
  BF_set_key(&bf_key, 16, key_str);
  unsigned char enc_buf[8];
  readable_dec(enc_s.c_str(), enc_buf);
  unsigned char org_buf[9];
  BF_ecb_encrypt(enc_buf, org_buf, &bf_key, BF_DECRYPT);
  org_buf[8] = '\0';
  cout << org_buf << endl;
  */
  return retval;
}

BOOL
af_open(struct Answer_file *af, const char *fname)
{
  af->fp = fopen(fname, "r");
  if (af->fp == NULL) return FALSE;
  return TRUE;
}

void
af_close(struct Answer_file *af)
{
  if (af->fp) fclose(af->fp);
}

BOOL
af_read_header(struct Answer_file *af)
{
  int e;
  if (fscanf(af->fp, "%d %d %d %d",
	       &(af->prob_num), &(af->w), &(af->h), &e) != 4)
    return FALSE;
  af->is_encripted = (e == 0 ? FALSE : TRUE);
  return TRUE;
}

BOOL
af_read_body(struct Answer_file *af, struct part_ans ans[])
{
  //XXX todo: encrypted
  int i;
  for (i = 0; i < af->w * af->h; i++) {
    char coord_str[9];
    int inner, left, upper;
    int x;
    int y;
    if (fscanf(af->fp, "%s %d %d %d", coord_str, &inner, &left, &upper) != 4)
      return FALSE;
    if (af->prob_num != cut_and_atoi(&coord_str[0], 2)) return FALSE;
    x = cut_and_atoi(&coord_str[2], 3);
    y = cut_and_atoi(&coord_str[5], 3);
    set_part_ans(&(ans[coord_a(x, y, af->w)]), inner, left, upper);
  }
  return TRUE;
}

void
grch_out(char* format, ...)
{
  va_list argp;
  fprintf(stderr, "[Grid Challenge] ");
  va_start(argp, format);
  vfprintf(stderr, format, argp);
  va_end(argp);
  fprintf(stderr, "\n");
}

int
cut_and_atoi(const char* str, int len)
{
  char copy_str[len + 1];
  memcpy(copy_str, str, len);
  copy_str[len] = '\0';
  return atoi(copy_str);
}

BOOL
answer_fname(const char* config_dir, const int pnum,
	     char *ans_fname)
{
  char pnum_buf[100];

  if (config_dir == NULL || config_dir[0] == '\0') return FALSE;

  strcpy(ans_fname, config_dir);
  if (ans_fname[strlen(ans_fname) - 1] != '/')
    strcat(ans_fname, "/");
  snprintf(pnum_buf, 100, "%02d.ans", pnum);
  strcat(ans_fname, pnum_buf);
  return TRUE;
}
/*
static void
hexout(const unsigned char* str, int len, char* retbuf)
{
  static const char xdc[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
  };

  int i;
  for (i = 0; i < len; i++) {
    retbuf[i * 2    ] = xdc[(str[i] >> 4) & 0x0f];
    retbuf[i * 2 + 1] = xdc[ str[i]       & 0x0f];
  }  retbuf[len * 2] = '\0';
}
*/
void
readable_enc(const unsigned char* str, int len, char* retbuf)
{
  int i;
  for (i = 0; i < len; i++) {
    retbuf[i * 2    ] = ((str[i] >> 4) & 0x0f) + 'a';
    retbuf[i * 2 + 1] = ( str[i]       & 0x0f) + 'a';
  }
  retbuf[len * 2] = '\0';
}
void
readable_dec(const char* str, unsigned char* retbuf)
{
  int i = 0;
  while (str[i] != '\0') {
    retbuf[i / 2] = ((str[i] - 'a') << 4) + (str[i + 1] - 'a');
    i += 2;
  }
}

void prepare_global_data()
{
  struct tm base_time; // 2004/12/01 00:00:00
  base_time.tm_sec = 0;
  base_time.tm_min = 0;
  base_time.tm_hour = 0;
  base_time.tm_mday = 1;
  base_time.tm_mon = 11;
  base_time.tm_year = 104;
  base_time.tm_isdst = 0;
  challenge_data.base_time = mktime(&base_time);
}

void
make_problem_key(int prob_num, char *retbuf)
{
  char time_str_buf[11];
  unsigned char md5_str[MD5_DIGEST_LENGTH + 1];
  char md5_readable_buf[33]; // md5の頭4バイトを使用

  snprintf(time_str_buf, 11, "%02d%08x",
	   prob_num,
	   (int)(challenge_data.start_time.tv_sec - challenge_data.base_time));
  // cf. 3600*24*100 = 0x83d600, 3600*24*365 = 0x1e13380, 6桁だとギリギリ?
  make_secret_md5(time_str_buf, 10, md5_str);
  readable_enc(md5_str, 16, md5_readable_buf);
  snprintf(retbuf, PROBLEM_KEY_LEN + 1, "%s%s",
	   time_str_buf, md5_readable_buf);
}

BOOL
check_problem_key(const char* problem_key)
{
  // XXX todo?: '-'以前のチェック
  
  // '-'以降のチェック
  const char *mp = strchr(problem_key, '-');
  unsigned char md5_str[MD5_DIGEST_LENGTH + 1];
  char md5_readable_buf[33];

  if (mp == NULL) return FALSE;
  mp++;
  make_secret_md5(mp, 10, md5_str);
  readable_enc(md5_str, 16, md5_readable_buf);
  if (strncmp(md5_readable_buf, mp + 10, 32) == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void
make_secret_md5(const char *data, int data_len, unsigned char *md5_str)
{
  char secret_key[] = "Internal error: ";
  MD5_CTX md5_ctx;
  MD5_Init(&md5_ctx);
  MD5_Update(&md5_ctx, data, data_len);
  MD5_Update(&md5_ctx, secret_key, strlen(secret_key));
  MD5_Final(md5_str, &md5_ctx);
}

const char *
file_encoding_key(int prob_num)
{
  const char *str;
  switch (prob_num) {
  case 1:
    str = 
      "c9988f6c2fcb4d492baece7fe9e52a3796e5e6fa";
    break;
  case 2:
    str =
      "267aeade4ada4933fa7bbf106885e963e03d46d9";
    break;
  case 3:
    str = 
      "07acb0527fc8341f727b6ac77b56815ae25cef46";
    break;
  case 4:
    str = 
      "b41283d2d4ca5e6e3259b7c01088e57b2e80f77a";
    break;
  case 5:
    str = 
      "fdea1c3b65a51f307d7f465ad06e5a3ea1085f92";
    break;
  case 6:
    str = 
      "e02750f448b185705a23996389418df770f24024";
    break;
  case 11:
    str = 
      "719ca94cb2546a952483c6dbbe6981d2cdd5c1b6";
    break;
  case 12:
    str = 
      "307ba680ad0e78a6e3ee60b2ba1ee8ccafdd3005";
    break;
  case 13:
    str = 
      "507688d1bc5db09d71c942fa7ccbd12fa419e511";
    break;
  case 14:
    str = 
      "c602b97864f28f9097468c71d9ac1021fb36c084";
    break;
  case 15:
    str = 
      "e599f350b0c33aeb0c5f5b15e35f4ba6824808c2";
    break;
  case 16:
    str = 
      "6efa6bd45638a1ac12b757be1d5c3f2a61c39d96";
    break;
  case 17:
    str = 
      "7d1b577431c90e8488a36a81973c965b0d6a85d7";
    break;
  case 18:
    str = 
      "d8795092af64980aa5d290666148866b3ab5c9d3";
    break;
  case 19:
    str = 
      "f040bd9dfd5eb5dd9849cedf655e2e34876e0f3d";
    break;
  case 20:
    str = 
      "890807f633a661e4de2dbe393d3b9b47e5a1a131";
    break;
  case 21:
    str = 
      "d68c61cf8f2ad458a1482ec019a4b16ee5250131";
    break;
  case 22:
    str = 
      "a052a97e6b9b67eb91d0ce08744441ab97404c66";
    break;
  case 23:
    str = 
      "e44ad7a398df6eaf1096a7bd7d9fa6618ad1df61";
    break;
  case 24:
    str = 
      "1efa45d10fb28dc078e1a4f4734f121e4ff00d46";
    break;
  case 25:
    str = 
      "8509e0fcec9581ebfe025a3547a6a2e74ab93eae";
    break;
  case 26:
    str = 
      "454053f52b8703bbbc0001600ee349f74ace811f";
    break;
  default:
    if (prob_num <= 10) {
      str = "00000000";
    } else {
      grch_out("Unknown problem no");
      str = "";
    }
    break;
  }
  return str;
}
