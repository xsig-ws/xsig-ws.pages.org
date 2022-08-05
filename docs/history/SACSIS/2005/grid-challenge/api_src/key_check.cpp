#define _XOPEN_SOURCE  

extern "C" {
#include "grichalle_api_private.h"
#include <openssl/md5.h>
#include <time.h>  
}
#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

bool
check_line(const string &line)
{
  int sl = line.find('/');
  if (sl == string::npos || sl < 1) {
    return false;
  }
  int data_end = sl - 1;
  string data = line.substr(0, data_end);
  unsigned char md5_str[MD5_DIGEST_LENGTH + 1];
  char md5_readable_buf[MD5_DIGEST_LENGTH * 2 + 1];
  make_secret_md5_(data.c_str(), data.length(), md5_str);
  readable_enc_(md5_str, MD5_DIGEST_LENGTH, md5_readable_buf);

  string calc_str = data + " / key = " + md5_readable_buf;
  if (line != calc_str) {
    return false;
  }
  return true;
}

struct Answer
{
  int prob_no;
  string at_str;
  time_t sec;
  Answer() : prob_no(0), sec(0) {}
  Answer(int pno, const string& at) : prob_no(pno), at_str(at) {
    struct tm tm;
    strptime(at.c_str(), "%b %d %H:%M:%S %Y", &tm);
    sec = mktime(&tm);
  }
  bool exists() const { return sec != 0; }
};

struct time_down_cmp
{
  bool operator()(const Answer& lhs, const Answer& rhs) {
    return lhs.sec > rhs.sec;
  }
};

bool
ans_check(const Answer ans[], int begin, int end, bool check_as)
{
  for (int i = begin; i < end; i++) {
    if (ans[i].exists() != check_as) return false;
  }
  return true;
}

int
main(int argc, char **argv)
{
  char buf[1024];
  Answer ans[10];
  
  while (cin.getline(buf, 1024)) {
    string raw_line(buf);
    int pb = raw_line.find("<<");
    int pe = raw_line.find(">>");
    if (pb == string::npos || pe == string::npos) {
      continue;
    }
    pb += 2;
    string line(raw_line, pb, pe - pb);
    if (!check_line(line)) {
      cout << "wrong key: " << raw_line << endl;
      return 1;
    }
    int prob_no;
    if (sscanf(line.c_str(), "Correct, prob %d", &prob_no) != 1) {
      cout << "cannot read problem number. giving up." << endl;
      return 1;
    }
    if (problem_number_check_(prob_no) != P_YOSEN) {
      cout << "problem number error: " << raw_line << endl;
      return 1;
    }
    int p = line.find("sec, at ");
    if (p == string::npos) {
       cout << "cannot read date. giving up." << endl;
      return 1;
    }
    p += sizeof("sec, at Thu ") - 1;
    Answer line_ans(prob_no,
		    line.substr(p, sizeof("Feb 03 16:11:40 2005") - 1));

    int index = prob_no - 11;
    if (ans[index].exists()) {
      if (ans[index].sec > line_ans.sec) {
	ans[index] = line_ans;
      }
    } else {
      ans[index] = line_ans;
    }
  }

  int ans_end = 0;
  if (ans[9].exists()) { // 上級
    if (!ans_check(ans, 0, 10, true)) {
      cout << "上級 解答不足" << endl; return 1;
    }
    cout << "上級 正解" << endl;
    ans_end = 10;
  } else { // 初級
    if (!ans_check(ans, 0, 5, true)) {
      cout << "初級 解答不足" << endl; return 1;
    }
    if (!ans_check(ans, 5, 10, false)) {
      cout << "上級 解答不足" << endl; return 1;
    }
    cout << "初級 正解" << endl;
    ans_end = 5;
  }

  sort(&(ans[0]), &(ans[ans_end]), time_down_cmp());
  for (int i = 0; i < ans_end; i++) {
    cout << ans[i].at_str << endl;
  }
}
