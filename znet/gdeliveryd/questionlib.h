#ifndef __QUESTION_LIB_H__
#define __QUESTION_LIB_H__

#include <vector>

#define QUESTION_SUM_OF_ARRAY 30

struct _QUESTION
{
	unsigned short* szQuestion;//问题的字符串指针
	char            cAnswer;   //答案：一个字节，从1位到第6位分别对应A，B，C，D，E，F。正确答案是1，错误答案为0
	int             nAnswer;   //答案的个数，最多6个
	unsigned short* pAnswer[6];//6个答案的字符串指针
};

struct _QUESTION_ARRAY
{
	std::vector<_QUESTION*> listQuestion;
};

class CQuestionLib
{
public:
	CQuestionLib();
	~CQuestionLib();

public:
	bool Load( const char *szPath );
	int  GetQtArrayCount(){ return m_listLib.size();}
	const _QUESTION_ARRAY* GetQtArray( int idx ){ return m_listLib[idx]; }
	
protected:
	void Release();
	
	std::vector<_QUESTION_ARRAY*> m_listLib;
	
};

#endif 
