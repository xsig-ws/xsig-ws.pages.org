#ifndef _GRICHALLE_API_H_
#define _GRICHALLE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
get_problem()��answer_problem()��Ʊ���ץ����ǸƤ֤���
�����exit���ƤϤ����ʤ�
*/

int get_problem(const int problem_number,
		const char* config_dir,
		const char** key_string,
		int* width, int* height);
/* problem_number : �����ֹ�
   config_dir�Ϥ���Ǥ������ե�������֤��Ƥ���ǥ��쥯�ȥ����ꤹ��
   ͽ�����辡�Ǥ�NULL�Ȥ��뤳�� 
   key_string ��static�˼��줿const char*���֤�

   return value : 0 / ����
		: not 0 / �۾�
*/

int get_problem_files(const int problem_number, const char* key_string,
		      const char* problem_dir,
		      const char** problem_files);
/* key_string ��start_contest�����������
   problem_files �ˤϥ�����˥���������ǽ�ʥե�����̾(�ե�ѥ�)���֤����
   �ƥե�����̾��ľ��ˤϥǥ�ߥ��Ȥ��� : (�����)�������Ƥ���
   problem_dir�Ϥ���Ǥ�����ե�������֤��Ƥ���ǥ��쥯�ȥ����ꤹ��
   ͽ�����辡�Ǥ�NULL�Ȥ��뤳�� 

   return value : 0 / ����
		: not 0 / �۾�
*/

#define GRCH_CORRECT	0
#define GRCH_OK		1
#define GRCH_WRONG	2
#define GRCH_NG		3

int answer_problem(const int problem_number, const char* key_string,
                   const int result,
		   const char* file_coordinates,
		   const char* config_dir);
/* result�ϲ����η�ʪ�ο�
   look_coordinates�ˤϲ����˻Ȥä����Ƥβ����κ�ɸ��������
   (0, 0), (1, 0), (2, 0),...,(1, 0), (1, 1),...
   �ν�ǡ����κ�ɸ�Υե��������Ѥ��Ƥ����'1'��
   ���Ѥ��Ƥ��ʤ����'0'��񤤤�ʸ�����Ϳ����
   config_dir�Ϥ���ǤǤ������ե�������֤��Ƥ���ǥ��쥯�ȥ����ꤹ��
   ͽ�����辡�Ǥ�NULL�Ȥ��뤳�� 

   return value : GRCH_CORRECT / ����
		: GRCH_OK      / ���Ѥ���problem_files�˴ؤ��Ƥ�����
		: GRCH_WRONG   / ������
		: GRCH_NG      / get_problem��Ʊ���Ρ��ɤǸƤ�Ǥ��ʤ�����
		                 �����ʸƤӽФ�
   ɸ����Ϥ˲������֤���ɷ����ˤ�����Τȡ���������θ������Ϥ����
   ��������θ���CORRECT�ξ��ˤΤ��ղ�
*/

#ifdef __cplusplus
};
#endif

#endif /* _GRICHALLE_API_H_ */
