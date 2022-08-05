#ifndef _GRICHALLE_API_PRIVATE_H_
#define _GRICHALLE_API_PRIVATE_H_

#include "grichalle_api_imple.h"
void make_secret_md5_(const char *data, int data_len,
		      unsigned char *md5_str);
void readable_enc_(const unsigned char* str, int len, char* retbuf);
int problem_number_check_(const int pnum);

#endif /* _GRICHALLE_API_PRIVATE_H_ */
