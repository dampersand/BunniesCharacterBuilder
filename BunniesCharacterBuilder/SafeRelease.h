//This header creates a simple template class that can be used to release
//a COM pointer without throwing an error.

#ifndef _SAFE_RELEASE_H
#define _SAFE_RELEASE_H

#include <Windows.h>

template <class T> 
void safeRelease(T **pointer)
{
	if (*pointer)
	{
		(*pointer)->Release();
		*pointer = NULL;
	}
}


#endif //_SAFE_RELEASE_H Guard Word