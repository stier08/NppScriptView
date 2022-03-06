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
#include "NppPluginAPI/include/menuCmdID.h"
#include "NppDockingTemplate/include/ScriptsViewDlg.h"
#include "NppPyScriptWinSupport/include/SampleDialogBox.h"
#include "NppPython/include/IPythonPluginManager.h"
#include "NppScintillaPython/include/PythonHandler.h"
#include "NppWrapper/include/INppWrapper.h"
#include <boost/shared_ptr.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include "NppPyScriptWinSupport/include/StackDump.h"
//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

#define DOCKABLE_DEMO_INDEX 15

#define _MY_STRINGIZE__(s) #s
#define _MY_STRINGIZE_(s) _MY_STRINGIZE__(s)
#define _MY_STRINGIZE(s) _MY_STRINGIZE_(s)

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

ScriptsViewDlg _scriptsViewDlg;
HINSTANCE g_hInstance;

void treeViewDlgEnsureCreated();
#pragma warning( push )

/*   Warnings disabled 
*   4592: 'g_pythonHandler': symbol will be dynamically initialized (implementation limitation)

*/
#pragma warning( disable : 4592)
boost::shared_ptr<NppPythonScript::PythonHandler> g_pythonHandler;
#pragma warning( pop )

/*   Warnings disabled
* 4702: unreachable code
*/
#pragma warning( disable : 4702)

HINSTANCE getHInstance()
{
	return g_hInstance;
}

void dllinitializeImpl(HANDLE hModule)
{
	try
	{
		g_hInstance = (HINSTANCE)hModule;
	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. dllinitializeImpl");
		OutputDebugStringA(ex.what());
	}
}

//
// Initialize your plugin data here
// It will be called while plugin loading   
bool dllinitializeSafe(HANDLE hModule)
{
	bool shexcep = false;
	__try
	{
		dllinitializeImpl(hModule);
	}
	__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
	{
		OutputDebugString(L"Structured Exception. dllinitializeSafe");
		shexcep = true;
	}
	return shexcep;
}


void dllinitialize(HANDLE hModule)
{
	if (dllinitializeSafe(hModule))
	{
		std::string msg = getSHExceptionString();
		OutputDebugStringA(msg.c_str());

	}
}

void dllCleanUpImpl()
{
	try

	{
		PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
		manager.finalize();
		g_pythonHandler.reset();
	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. dllCleanUpImpl");
		OutputDebugStringA(ex.what());
	}
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
bool dllCleanUpSafe()
{
	bool shexcp = true;
	__try
	{
		dllCleanUpImpl();
		shexcp = false;
	}
	__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
	{
		OutputDebugString(L"Structured Exception. dllCleanUpSafe");

	}
	return shexcp;
}

void dllCleanUp()
{
	if (dllCleanUpSafe())
	{
		std::string msg = getSHExceptionString();
		OutputDebugStringA(msg.c_str());
	}

}


void initPythonPluginsImpl()
{
	try
	{
		g_pythonHandler = boost::shared_ptr<NppPythonScript::PythonHandler>(new NppPythonScript::PythonHandler((HINSTANCE)getHInstance(), nppData._nppHandle, nppData._scintillaMainHandle, nppData._scintillaSecondHandle));

		NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
		nppwrapper.initialize((HINSTANCE)getHInstance(), 
			nppData._nppHandle,
			nppData._scintillaMainHandle,
			nppData._scintillaSecondHandle
			);

		PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();

		manager.preinitCppPythonModules();

		if (g_pythonHandler)
		{
			g_pythonHandler->initPython();
		}
		manager.initialize();
		treeViewDlgEnsureCreated();
		manager.set_event_sink(&_scriptsViewDlg);

	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. initPythonPluginsImpl");
		OutputDebugStringA(ex.what());
	}

}

void reloadPythonScriptsImpl()
{
	try
	{
		PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
		manager.reloadScripts();
	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. reloadPythonScriptsImpl");
		OutputDebugStringA(ex.what());
	}
}

void reloadPythonScripts()
{
	__try
	{
		reloadPythonScriptsImpl();
	}
	__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
	{
		OutputDebugString(L"Structured Exception. reloadPythonScripts");
		OutputDebugStringA(getSHExceptionStringStrA());
	}

}

bool initPythonPlugins()
{
	__try
	{
		initPythonPluginsImpl();
		return true;
	}
	__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
	{
		OutputDebugString(L"Structured Exception. initPythonPlugins");
		OutputDebugStringA(getSHExceptionStringStrA());

		return false;
	}

}
//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("NppPyStandalonePlugin About"), createAboutDoc, NULL, false);
    setCommand(1, TEXT("NppPyStandalonePlugin About Dlg"), showAboutDlg, NULL, false);
	setCommand(2, TEXT("Reload Scripts"), reloadScripts, NULL, false);
	setCommand(3, TEXT("Run Current File"), pythonRuntCurrentFile, NULL, false);
	setCommand(4, TEXT("Py Executre Selection"), pythonRuntSelection, NULL, false);
	setCommand(5, TEXT("Tree View Dialog"), treeViewDlg, NULL, false);

	if (initPythonPlugins())
	{
		reloadPythonScripts();
	}
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

std::string getBuildEnv()
{
	#pragma warning( push )
#pragma warning( disable : 4129)


	

	std::stringstream ss;
	ss << "NppPyStandalonePlugin" << std::endl
		<< "Build environment setup" << std::endl
#ifdef _DEBUG
		<< "DEBUG BUILD" << std::endl
#else
		<< "RELEASE BUILD" << std::endl
#endif		
		<< "Build timestamp " << __DATE__ << " " __TIME__  << std::endl
		<< "BoostFileVer : " << _MY_STRINGIZE(PROP_BoostFileVer) << std::endl
		<< "ConfigurationPlatformToolset : " << _MY_STRINGIZE(PROP_PlatformToolset) << std::endl
		<< "VSVer : " << _MY_STRINGIZE(PROP_VSVer) << std::endl
		<< "VSNum : " << _MY_STRINGIZE(PROP_VSNum) << std::endl
		<< "BoostVer : " << _MY_STRINGIZE(PROP_BoostVer) << std::endl
		<< "PythonMagorVer : " << _MY_STRINGIZE(PROP_PythonMagorVer) << std::endl
		<< "PythonMinorVer : " << _MY_STRINGIZE(PROP_PythonMinorVer) << std::endl
		<< "PythonVer : " << _MY_STRINGIZE(PROP_PythonVer) << std::endl
		<< "BoostPythonTag : " << _MY_STRINGIZE(PROP_BoostPythonTag) << std::endl;
		
/*		
		<< "PythonFolder : " << _MY_STRINGIZE(PROP_PythonFolder) << std::endl
		<< "PythonInclude : " << _MY_STRINGIZE(PROP_PythonInclude) << std::endl
		<< "ThirdPartyLibs : " << _MY_STRINGIZE(PROP_ThirdPartyLibs) << std::endl
		<< "WTLFolder : " << _MY_STRINGIZE(PROP_WTLFolder) << std::endl
		<< "BoostFolder : " << _MY_STRINGIZE(PROP_BoostFolder) << std::endl
		<< "BoostInclude : " << _MY_STRINGIZE(PROP_BoostInclude) << std::endl;
*/


#pragma warning( pop )
return ss.str();
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void createAboutDoc()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	CString cEnv;
	std::string envstr = getBuildEnv();

	cEnv = CString(envstr.c_str());	
    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
	// https://www.arclab.com/en/kb/cppmfc/convert-cstring-unicode-utf-16le-to-utf-8-and-reverse.html
	CStringA utf8;
    int cc=0;
    // get length (cc) of the new multibyte string excluding the \0 terminator first
    if ((cc = WideCharToMultiByte(CP_UTF8, 0, cEnv, -1, NULL, 0, 0, 0) - 1) > 0)
    {
        // convert
        char *buf = utf8.GetBuffer(cc);
        if (buf) WideCharToMultiByte(CP_UTF8, 0, cEnv, -1, buf, cc, 0, 0);
        utf8.ReleaseBuffer();
    }
	
	char* charPtr = (char*)utf8.GetBuffer(utf8.GetLength());
	
	
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)	charPtr);
}

void showAboutDlg()
{
	CString cEnv;
	std::string envstr = getBuildEnv();
	cEnv = CString(envstr.c_str());	
	
    ::MessageBox(NULL, (LPCWSTR)	cEnv, TEXT("NppPyStandalonePlugin Build environment"), MB_OK);
}


void pythonRuntCurrentFileImpl()
{

	try
	{
		PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
		NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
		std::wstring path(nppwrapper.getActiveDocumentFilePathW());
		if (!path.empty())
		{
			manager.run_python_file(path);
		}
	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. pythonRuntCurrentFileImpl");
		OutputDebugStringA(ex.what());
	}


}

void pythonRuntCurrentFile()
{
	bool shexp = true;
	__try
	{
		pythonRuntCurrentFileImpl();
		shexp = false;
	}
	__except (MSJUnhandledExceptionFilter( GetExceptionInformation() ) )
	{
		OutputDebugString(L"Structured Exception. pythonRuntCurrentFileImpl");
	}
	if (shexp)
	{
		OutputDebugStringA(getSHExceptionStringStrA());
	}
}

void pythonRuntSelection()
{
	PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
	NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
	std::wstring selection(nppwrapper.getSelectionTextW());
	if (!selection.empty())
	{
		manager.python_exec(selection);
	}
}

void reloadScriptsImpl()
{
	try
	{
		PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
		manager.reloadScripts();
	}
	catch (std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. reloadScriptsImpl");
		OutputDebugStringA(ex.what());
	}
}

void reloadScripts()
{
	__try
	{
		reloadScriptsImpl();
	}
	__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
	{
		OutputDebugString(L"Structured Exception. reloadScripts");
		OutputDebugStringA(getSHExceptionStringStrA());
	}
}


void treeViewDlgEnsureCreated()
{
	_scriptsViewDlg.setParent(nppData._nppHandle);
	if (!_scriptsViewDlg.isCreated())
	{
		tTbData	data = { 0 };

		_scriptsViewDlg.create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_RIGHT;

		data.pszModuleName = _scriptsViewDlg.getPluginFileName();

		// the dlgDlg should be the index of funcItem where the current function pointer is
		// in this case is DOCKABLE_DEMO_INDEX
		data.dlgID = DOCKABLE_DEMO_INDEX;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	_scriptsViewDlg.display();

	// WindowSupport::createTreeViewDialogBox(getHInstance(), nppData._nppHandle);
}

void treeViewDlg()
{
	treeViewDlgEnsureCreated();
}
