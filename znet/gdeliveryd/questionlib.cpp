#include "questionlib.h"

#include <stdio.h>


CQuestionLib::CQuestionLib()
{

}

CQuestionLib::~CQuestionLib()
{
	Release();
}

bool CQuestionLib::Load( const char *szPath )
{
	Release();
	
	FILE* pFile = fopen(szPath,"rb");
	if(NULL == pFile) return false;

	_QUESTION_ARRAY *pAY = new _QUESTION_ARRAY;
	
	int nLen,nChar,nAnswer,nCount = 0;
	char cAnswer;
	fread(&nLen,sizeof(int),1,pFile);
	for( int i = 0; i < nLen; ++i)
	{
		_QUESTION *pQN = new _QUESTION;
		unsigned short *pQuestion = NULL;
		for( int j = 0; j < 6; ++j) pQN->pAnswer[j] = NULL;

		fread(&cAnswer,1,1,pFile);
		fread(&nChar,sizeof(int),1,pFile);
		if(nChar > 0) 
		{
			pQuestion = new unsigned short[nChar+1];
			fread(pQuestion,sizeof(unsigned short)*nChar,1,pFile);
			pQuestion[nChar] = 0;
		}

		fread(&nAnswer,sizeof(int),1,pFile);
		for(int j = 0; j < nAnswer; ++j)
		{
			int nAnswerLen;
			fread(&nAnswerLen,sizeof(int),1,pFile);
			if(nAnswerLen > 0)
			{
				unsigned short *pAnswer = new unsigned short[nAnswerLen + 1];
				fread(pAnswer,sizeof(unsigned short)*nAnswerLen,1,pFile);
				pAnswer[nAnswerLen] = 0;
				pQN->pAnswer[j] = pAnswer; 
			}
		}

		pQN->cAnswer = cAnswer;
		pQN->szQuestion = pQuestion;
		pQN->nAnswer = nAnswer;
		pAY->listQuestion.push_back(pQN);
		nCount++;

		if(nCount >= QUESTION_SUM_OF_ARRAY)
		{
			m_listLib.push_back(pAY);
			pAY = new _QUESTION_ARRAY;
			nCount = 0;
		}
	}

	if( nCount < QUESTION_SUM_OF_ARRAY && nCount > 0)
	{
		int len2 = pAY->listQuestion.size();
		for( int j = 0; j < len2; ++j)
		{
			_QUESTION *pQuestion = pAY->listQuestion[j];
			delete []pQuestion->szQuestion;
			for( int k = 0; k < pQuestion->nAnswer; ++k)
				delete []pQuestion->pAnswer[k];
			delete pQuestion;
		}
		pAY->listQuestion.clear();
		delete pAY;
	}

	fclose(pFile);
	if(m_listLib.size() < 1) return false;
	return true;
}

void CQuestionLib::Release()
{
	int len1 = m_listLib.size();
	for( int i = 0; i < len1; ++i)
	{
		_QUESTION_ARRAY *pArray = m_listLib[i];
		
		int len2 = pArray->listQuestion.size();
		for( int j = 0; j < len2; ++j)
		{
			_QUESTION *pQuestion = pArray->listQuestion[j];
			delete []pQuestion->szQuestion;
			for( int k = 0; k < pQuestion->nAnswer; ++k)
				delete []pQuestion->pAnswer[k];
			delete pQuestion;
		}
		pArray->listQuestion.clear();
		delete pArray;
	}
	m_listLib.clear();
}

