/* Copyright (c) 2002-2012 Croteam Ltd.
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

/*
source https://github.com/Croteam-official/Serious-Engine/blob/master/Sources/Engine/Base/StackDump.cpp
*/


#include "stdafx.h"

//#include <Engine/Base/Console_Internal.h>

//#include <Engine/Build.h>

#pragma warning( push )

/*   Warnings disabled
*   4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

*/
#pragma warning( disable : 4996)

#include <iostream>
#include <iomanip>
#include <cassert>
#include <stdlib.h>

#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time/local_time.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/filesystem/operations.hpp>
#include <filesystem>

#include "NppPyScriptWinSupport/include/StackDump.h"

//==========================================
// Matt Pietrek
// Microsoft Systems Journal, May 1997
// FILE: MSJEXHND.CPP

class MSJExceptionHandler
{
public:

	MSJExceptionHandler();
	~MSJExceptionHandler();

	void SetLogFileName(const char* pszLogFileName);
	static std::stringstream m_exceptionReport;
	// entry point where control comes on an unhandled exception
	static LONG WINAPI MSJUnhandledExceptionFilter(
		PEXCEPTION_POINTERS pExceptionInfo);

private:



	// where report info is extracted and generated
	static void GenerateExceptionReport(PEXCEPTION_POINTERS pExceptionInfo);

	// Helper functions
	static const char* GetExceptionString(DWORD dwCode);
	static BOOL GetLogicalAddress(PVOID addr, char* szModule, DWORD len,
		DWORD& section, DWORD& offset);
	static void IntelStackWalk(PCONTEXT pContext);
	static int __cdecl _tprintf(const char * format, ...);

	// Variables used by the class
	static char m_szLogFileName[MAX_PATH];
	static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
	static HANDLE m_hReportFile;


};

//extern MSJExceptionHandler g_MSJExceptionHandler;  // global instance of class

MSJExceptionHandler g_MSJExceptionHandler;  // Declare global instance of class

LONG WINAPI MSJUnhandledExceptionFilter(
	PEXCEPTION_POINTERS pExceptionInfo)
{
	return g_MSJExceptionHandler.MSJUnhandledExceptionFilter(pExceptionInfo);

}


std::string getSHExceptionString()
{
	return g_MSJExceptionHandler.m_exceptionReport.str();
}

const char* getSHExceptionStringStrA()
{
	return g_MSJExceptionHandler.m_exceptionReport.str().c_str();
}

											//============================== Global Variables =============================

											//
											// Declare the static variables of the MSJExceptionHandler class
											//
char MSJExceptionHandler::m_szLogFileName[MAX_PATH];
LPTOP_LEVEL_EXCEPTION_FILTER MSJExceptionHandler::m_previousFilter;
HANDLE MSJExceptionHandler::m_hReportFile;
std::stringstream MSJExceptionHandler::m_exceptionReport;
//============================== Class Methods =============================

//=============
// Constructor
//=============
MSJExceptionHandler::MSJExceptionHandler()
{
	OutputDebugString(L"[Started] MSJExceptionHandler::MSJExceptionHandler");
	// Install the unhandled exception filter function
	m_previousFilter = SetUnhandledExceptionFilter(MSJUnhandledExceptionFilter);

	// Figure out what the report file will be named, and store it away
	GetModuleFileNameA(0, m_szLogFileName, MAX_PATH);

	// Look for the '.' before the "EXE" extension.  Replace the extension
	// with "RPT"
	char* pszDot = strrchr(m_szLogFileName, '.');
	if (pszDot)
	{
		pszDot++;   // Advance past the '.'
		if (strlen(pszDot) >= 3)
			strcpy(pszDot, "RPT");   // "RPT" -> "Report"
	}
	OutputDebugString(L"[Finished] MSJExceptionHandler::MSJExceptionHandler");
}

//============
// Destructor
//============
MSJExceptionHandler::~MSJExceptionHandler()
{
	OutputDebugString(L"[Started] MSJExceptionHandler::~MSJExceptionHandler");
	SetUnhandledExceptionFilter(m_previousFilter);
	OutputDebugString(L"[Finished] MSJExceptionHandler::~MSJExceptionHandler");
}

//==============================================================
// Lets user change the name of the report file to be generated
//==============================================================
void MSJExceptionHandler::SetLogFileName(const char* pszLogFileName)
{
	strcpy(m_szLogFileName, pszLogFileName);
}

//===========================================================
// Entry point where control comes on an unhandled exception
//===========================================================
LONG WINAPI MSJExceptionHandler::MSJUnhandledExceptionFilter(
	PEXCEPTION_POINTERS pExceptionInfo)
{
	OutputDebugString(L"[Started] MSJExceptionHandler::MSJUnhandledExceptionFilter");

#pragma warning( push )

	/*   Warnings disabled
	*   4996: 'getenv': This function or variable may be unsafe. Consider using _dupenv_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
	*/
#pragma warning( disable : 4996)
	const char* env = std::getenv("PORTABLE_WS_TMP_HOME");
#pragma warning( pop )
	if (!env)
	{
#pragma warning( push )

		/*   Warnings disabled
		*   4996: 'getenv': This function or variable may be unsafe. Consider using _dupenv_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
		*/
#pragma warning( disable : 4996)
		 env = std::getenv("TEMP");
#pragma warning( pop )
	}
	if(env)
	{
		OutputDebugString(L"Saving exceptin info under");
		OutputDebugStringA(env);
		boost::filesystem::path dir(env);

		std::stringstream str;

		boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
		str << boost::format("%d-%02d-%02d--%02d-%02d-%02d")
			% currentTime.date().year()
			% currentTime.date().month()
			% currentTime.date().day()
			% currentTime.time_of_day().hours()
			% currentTime.time_of_day().minutes()
			% currentTime.time_of_day().seconds();
		std::string stime = str.str();
		boost::filesystem::path folder = dir / "UnhandledException";
		if (!boost::filesystem::exists(folder))
		{
			boost::filesystem::create_directories(folder);
		}
		
		boost::filesystem::path p = folder / (stime + ".info");


		

		m_hReportFile = CreateFileA(
			p.string().c_str()
			/*m_szLogFileName*/,
			GENERIC_WRITE,
			0,
			0,
			OPEN_ALWAYS,
			FILE_FLAG_WRITE_THROUGH,
			0);

		if (m_hReportFile)
		{
			SetFilePointer(m_hReportFile, 0, 0, FILE_END);

			GenerateExceptionReport(pExceptionInfo);

			CloseHandle(m_hReportFile);
			m_hReportFile = 0;

			std::fstream of(p.string().c_str());
			of << m_exceptionReport.str();
		}

	}
	else
	{
		OutputDebugString(L"Failed to identify temp folder");
	}

	OutputDebugString(L"[Finished] MSJExceptionHandler::MSJUnhandledExceptionFilter");


//	extern void EnableWindowsKeys(void);
//	EnableWindowsKeys();

	/*if (m_previousFilter)
		return m_previousFilter(pExceptionInfo);
	else*/
	/*	return EXCEPTION_CONTINUE_SEARCH;*/
	// https://docs.microsoft.com/en-us/windows/win32/debug/using-an-exception-handler
	return EXCEPTION_EXECUTE_HANDLER;
}

//===========================================================================
// Open the report file, and write the desired information to it.  Called by
// MSJUnhandledExceptionFilter
//===========================================================================
#ifdef _M_IX86
void MSJExceptionHandler::GenerateExceptionReport(
	PEXCEPTION_POINTERS pExceptionInfo)
{
  // Intel Only!
	m_exceptionReport = std::stringstream();
	// Start out with a banner
	m_exceptionReport << "//====================================================" << std::endl;
	char strTime[80];
	_strtime(strTime);
	char strDate[80];
	_strdate(strDate);
	m_exceptionReport << "Crashed at: " <<  strDate << " " << strTime << std::endl;;

	PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;

	// First print information about the type of fault
	m_exceptionReport << "Exception code: " << "0x" 
		<< pExceptionRecord->ExceptionCode 
		<< " : " << GetExceptionString(pExceptionRecord->ExceptionCode);

	// Now print information about where the fault occured
	char szFaultingModule[MAX_PATH];
	DWORD section, offset;
	GetLogicalAddress(pExceptionRecord->ExceptionAddress,
		szFaultingModule,
		sizeof(szFaultingModule),
		section, offset);

	m_exceptionReport <<  "Fault address: 0x"
		<< pExceptionRecord->ExceptionAddress 
		<<  section << " " << offset << " " << szFaultingModule << std::endl;;

	PCONTEXT pCtx = pExceptionInfo->ContextRecord;

	// Show the registers

	char buf[256];
	/*int rc =*/ sprintf_s(buf, sizeof(buf),
		"EAX:%08X\tEBX:%08X\n"
		"ECX:%08X\tEDX:%08X\n"
		"ESI:%08X\tEDI:%08X\n"
		"\n"
		"CS:EIP:%04X:%08X\n"
		"SS:ESP:%04X:%08X\tEBP:%08X\n"
		"\n"
		"DS:%04X\tES:%04X\tFS:%04X\tGS:%04X\n"
		"Flags:%08X\n",
		pCtx->Eax, pCtx->Ebx,
		pCtx->Ecx, pCtx->Edx,
		pCtx->Esi, pCtx->Edi,
		pCtx->SegCs, pCtx->Eip,
		pCtx->SegSs, pCtx->Esp, pCtx->Ebp,
		pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs,
		pCtx->EFlags
	);
	m_exceptionReport << "Registers:" << std::endl;
	m_exceptionReport << buf;
	m_exceptionReport << " Flags : 0x"  << pCtx->EFlags << std::endl;

	// Walk the stack using x86 specific code
	IntelStackWalk(pCtx);


	_tprintf("\n");
}
#else
  void MSJExceptionHandler::GenerateExceptionReport(
	PEXCEPTION_POINTERS /*pExceptionInfo**/)
  {
  }
#endif
//======================================================================
// Given an exception code, returns a pointer to a static string with a
// description of the exception
//======================================================================
const char* MSJExceptionHandler::GetExceptionString(DWORD dwCode)
{
#define EXCEPTION( x ) case EXCEPTION_##x: return #x;

	switch (dwCode)
	{
		EXCEPTION(ACCESS_VIOLATION)
			EXCEPTION(DATATYPE_MISALIGNMENT)
	case EXCEPTION_BREAKPOINT: return "BREAKPOINT";
		//EXCEPTION( BREAKPOINT )
		EXCEPTION(SINGLE_STEP)
			EXCEPTION(ARRAY_BOUNDS_EXCEEDED)
			EXCEPTION(FLT_DENORMAL_OPERAND)
			EXCEPTION(FLT_DIVIDE_BY_ZERO)
			EXCEPTION(FLT_INEXACT_RESULT)
			EXCEPTION(FLT_INVALID_OPERATION)
			EXCEPTION(FLT_OVERFLOW)
			EXCEPTION(FLT_STACK_CHECK)
			EXCEPTION(FLT_UNDERFLOW)
			EXCEPTION(INT_DIVIDE_BY_ZERO)
			EXCEPTION(INT_OVERFLOW)
			EXCEPTION(PRIV_INSTRUCTION)
			EXCEPTION(IN_PAGE_ERROR)
			EXCEPTION(ILLEGAL_INSTRUCTION)
			EXCEPTION(NONCONTINUABLE_EXCEPTION)
			EXCEPTION(STACK_OVERFLOW)
			EXCEPTION(INVALID_DISPOSITION)
			EXCEPTION(GUARD_PAGE)
			EXCEPTION(INVALID_HANDLE)
	}

	// If not one of the "known" exceptions, try to get the string
	// from NTDLL.DLL's message table.

	static char szBuffer[512] = { 0 };

	FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
		GetModuleHandleA("NTDLL.DLL"),
		dwCode, 0, szBuffer, sizeof(szBuffer), 0);

	return szBuffer;
}

//==============================================================================
// Given a linear address, locates the module, section, and offset containing
// that address.
//
// Note: the szModule paramater buffer is an output buffer of length specified
// by the len parameter (in characters!)
#ifdef _M_IX86
//==============================================================================
BOOL MSJExceptionHandler::GetLogicalAddress(
	PVOID addr, char* szModule, DWORD len, DWORD& section, DWORD& offset)
{
	MEMORY_BASIC_INFORMATION mbi;

	if (!VirtualQuery(addr, &mbi, sizeof(mbi)))
		return FALSE;

	DWORD hMod = (DWORD)mbi.AllocationBase;

	if (!GetModuleFileNameA((HMODULE)hMod, szModule, len))
		return FALSE;

	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);

	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHdr);

	DWORD rva = (DWORD)addr - hMod; // RVA is offset from module load address

									// Iterate through the section table, looking for the one that encompasses
									// the linear address.
	for (unsigned i = 0;
		i < pNtHdr->FileHeader.NumberOfSections;
		i++, pSection++)
	{
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart
			+ std::max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

		// Is the address in this section???
		if ((rva >= sectionStart) && (rva <= sectionEnd))
		{
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			section = i + 1;
			offset = rva - sectionStart;
			return TRUE;
		}
	}

	return FALSE;   // Should never get here!
}
#else
BOOL MSJExceptionHandler::GetLogicalAddress(
	PVOID /*addr*/,
	char* /*szModule*/,
	DWORD /*len*/,
	DWORD& /*section*/,
	DWORD& /*offset*/)
{
return FALSE;   // Should never get here!
}
#endif
//============================================================
// Walks the stack, and writes the results to the report file
//============================================================
#ifdef _M_IX86
void MSJExceptionHandler::IntelStackWalk(PCONTEXT pContext)
{

	m_exceptionReport << std::endl << "manual stack frame walk begin:" << std::endl ;

	m_exceptionReport <<  "Address   Frame     Logical addr  Module" << std::endl;

	DWORD pc = pContext->Eip;
	PDWORD pFrame, pPrevFrame;

	pFrame = (PDWORD)pContext->Ebp;


	for (size_t iDepth = 0; iDepth<100; iDepth++)
	{
		char szModule[MAX_PATH] = "";
		DWORD section = 0, offset = 0;
		m_exceptionReport << boost::format("%08X %08X ") % pc %  pFrame;

		//DWORD symDisplacement = 0;  // Displacement of the input address,
									// relative to the start of the symbol

		GetLogicalAddress((PVOID)pc, szModule, sizeof(szModule), section, offset);

		m_exceptionReport << boost::format( "$adr: %s %04X:%08X") % szModule %  section % offset;


		m_exceptionReport << std::endl;

		pc = pFrame[1];

		pPrevFrame = pFrame;

		pFrame = (PDWORD)pFrame[0]; // proceed to next higher frame on stack

		if ((DWORD)pFrame & 3)    // Frame pointer must be aligned on a
			break;                  // DWORD boundary.  Bail if not so.

		if (pFrame <= pPrevFrame)
			break;

		// Can two DWORDs be read from the supposed frame address?
		if (IsBadWritePtr(pFrame, sizeof(PVOID) * 2))
			break;

	};
	m_exceptionReport << std::endl << "manual stack frame walk end:" << std::endl;
}
#else
void MSJExceptionHandler::IntelStackWalk(PCONTEXT /*pContext*/)
{
}
#endif
//============================================================================
// Helper function that writes to the report file, and allows the user to use
// printf style formating
//============================================================================
int __cdecl MSJExceptionHandler::_tprintf(const char * format, ...)
{
	char szBuff[1024];
	int retValue;
	DWORD cbWritten;
	va_list argptr;

	va_start(argptr, format);
	retValue = wvsprintfA(szBuff, format, argptr);
	va_end(argptr);

	WriteFile(m_hReportFile, szBuff, retValue * sizeof(char), &cbWritten, 0);

	return retValue;
}


#pragma warning( pop )