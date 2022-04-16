//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
#include "NppPluginScriptView/include/PluginDefinition.h"


BOOL  DllMainImpl(HANDLE hModule, DWORD  reasonForCall, LPVOID /*lpReserved*/)
{
	switch (reasonForCall)
	{
	case DLL_PROCESS_ATTACH:
		dllinitialize(hModule);
		break;

	case DLL_PROCESS_DETACH:
		dllCleanUp();
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  reasonForCall, LPVOID lpReserved)
{
	BOOL ret = FALSE;
	try
	{
		ret =  DllMainImpl(hModule, reasonForCall, lpReserved);
	}
	catch (std::exception & ex)
	{
		::MessageBoxA(NULL, ex.what(), "Exception", MB_OK);

		OutputDebugString(L"Exception. DllMain");
		OutputDebugStringA(ex.what());

		ret= FALSE;
	}
	return ret;
}


