#ifndef NPP_PYSCRIPT_WIN_SUPPORT_STACKDUMP_H_GUARD
#define NPP_PYSCRIPT_WIN_SUPPORT_STACKDUMP_H_GUARD


#include <string>

std::string getSHExceptionString();

LONG WINAPI MSJUnhandledExceptionFilter(
	PEXCEPTION_POINTERS pExceptionInfo);

const char* getSHExceptionStringStrA();

#endif