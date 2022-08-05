#include "grid_challenge_api.h"

int
main(int argc, char **argv)
{
  if (argc < 4) return 1;
  
  int pnum = atoi(argv[1]);

  char buf[100];
  snprintf(buf, 100, "/home/yokoyama/program/grichalle/grch-otameshi/data%02d", pnum);
  const char* key_str;
  int col, row;
  if (get_problem(pnum, buf, &key_str, &col, &row) != 0) {
    //    cout << "error" << endl;
  }
  //  cout << col << " " << row << " " << key_str << endl;
  printf("%d, %d, %s\n", col, row, key_str);
  const char* prob_f;
  if (get_problem_files(pnum, key_str, buf, &prob_f) != 0) {
    //    cout << "error" << endl;
  }
  //  cout << prob_f << endl;

  sleep(3);
  int result = atoi(argv[2]);
  const char dummy_key_str[] = "00-hogehoge";
  int r = answer_problem(pnum, key_str, result, argv[3], buf);

  char *retstr[] = {"CORRECT", "OK", "WRONG", "NG"};
  printf("%s\n", retstr[r]);
  
  /*  switch (r) {
  case CORRECT:
    cout << "CORRECT" << endl; break;
  case OK:
    cout << "OK" << endl; break;
  case WRONG:
    cout << "WRONG" << endl; break;
  case NG:
    cout << "NG" << endl; break;
    }*/
}
