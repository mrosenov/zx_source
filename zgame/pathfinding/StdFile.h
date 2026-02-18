/********************************************************************
	created:	2008/06/04
	author:		liudong
	
	purpose:	std file
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#ifndef  _STD_FILE_H_
#define  _STD_FILE_H_

#include <stdio.h>
#include "AssertLowPriority.h"
#include "IFile.h"

class StdFile: public IFile
{

private:
	FILE * fp;
public:
	StdFile()
	{
		fp = NULL;
	}
	~StdFile()
	{
		Close();
	}

	inline	bool  Open(const char * szPath, unsigned long flags);
	inline  bool  Close();
	inline  size_t Read(void * buffer, size_t size);
	inline	size_t Write(const void * buffer, size_t size);
};

inline	bool  StdFile::Open(const char * szPath, unsigned long flags)
{
	Close();

	char mode[4] = {0,0,0,0};
	
	//read write
	if (IFILE_READ&flags && IFILE_WRITE&flags)
	{
		mode[0] = 'w';
		mode[2] = '+';
	}
	else if (IFILE_READ&flags)
	{
		mode[0] = 'r';
	}
    else if (IFILE_WRITE&flags)
	{
		mode[0] = 'w';
	}
	else
	{
		return false;
	}

	// text or binary
	if (IFILE_TEXT&flags && IFILE_BIN&flags)
	{
		return false;
	}
	else if (IFILE_TEXT&flags)
	{
		mode[1] = 't';
	}
    else if (IFILE_BIN&flags)
	{
		mode[1] = 'b';
	}
	else
	{
		//If t or b is not given in mode, the default translation mode is defined by the global variable _fmode.
		return false;
	}

	fp = fopen(szPath,mode);
	return (fp!=NULL);
}

inline	bool  StdFile::Close()
{
	if (!fp)
	{
		return true;
	}
	if(fclose(fp))
	{
		return false;
	}
	fp = NULL;
	return true;
}

inline	size_t StdFile::Read(void * buffer, size_t size)
{
	if (!fp || !buffer || 0==size)
	{
		ASSERT_LOW_PRIORITY(0);
		return 0;
	}
	return fread(buffer,1,size,fp);
}

inline	size_t StdFile::Write(const void * buffer, size_t size)
{
	if (!fp || !buffer || 0==size)
	{
		ASSERT_LOW_PRIORITY(0);
		return 0;
	}
	return fwrite(buffer,1,size,fp);
}

#endif 

