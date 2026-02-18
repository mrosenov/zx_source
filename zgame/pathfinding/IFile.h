/********************************************************************
	created:	2008/06/04
	author:		kuiwu
	
	purpose:	a simple interface for file
	Copyright (C) 2008 - All Rights Reserved
*********************************************************************/
#ifndef  _IFILE_H_
#define  _IFILE_H_

#define  IFILE_TEXT     0x00000001
#define  IFILE_BIN      0x00000002
#define  IFILE_READ     0x00000004
#define  IFILE_WRITE    0x00000008

class IFile
{
public:
	IFile(){};
	virtual ~IFile(){}

	//flags: example: IFILE_TEXT , IFILE_READ ......
	virtual  bool   Open(const char * szPath, unsigned long flags) = 0;
	virtual  bool   Close() = 0;

	//size  : Maximum number of bytes to be read.
	//return: The number of bytes actually read.
	virtual  size_t Read(void * buffer, size_t size) = 0;

	//size  : Maximum number of bytes to be written.
	//return: The number of bytes actually written.
	virtual  size_t Write(const void * buffer, size_t size) = 0;
protected:

};

#endif

