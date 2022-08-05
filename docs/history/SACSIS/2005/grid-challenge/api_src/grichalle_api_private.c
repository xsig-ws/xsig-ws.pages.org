#include "grichalle_api_imple.c"

void
make_secret_md5_(const char *data, int data_len,
		 unsigned char *md5_str)
{
  make_secret_md5(data, data_len, md5_str);
}

void
readable_enc_(const unsigned char* str, int len, char* retbuf)
{
  readable_enc(str, len, retbuf);
}

int problem_number_check_(const int pnum)
{
  return problem_number_check(pnum);
}

