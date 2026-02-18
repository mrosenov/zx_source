/********************************************************************
	created:	2009/01/13
	author:		liudong
	
	purpose:	macro about assert.

*********************************************************************/

#ifndef _ASSERT_LOW_PRIORITY_H_
#define _ASSERT_LOW_PRIORITY_H_



#ifdef _ASSERT_STRICT_ 
    #define ASSERT_LOW_PRIORITY(x) ASSERT(x)
#else
    #define ASSERT_LOW_PRIORITY(x) ASSERT(true||(x));//to avoid warning: no used variable "x".
#endif






#endif

