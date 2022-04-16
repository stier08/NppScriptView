#include "stdafx.h"

#include "NppScriptEngine/include/ScriptEngine.h"
#include "NppScriptEngine/include/ScriptEnginePluginManager.h"
#include "ScriptManager/include/IScriptRegistry.h"
#include "ScriptManager/include/StringSupport.h"


#include "NppScriptEngine/include/ScriptEnginenpp.h"

#include "NppScriptWinSupport/include/StackDump.h"

#include "PythonScript/include/NppPythonScript.h"

#include  <fstream>
#include  <sstream>
#include  <boost/shared_ptr.hpp>
#include <codecvt>

#pragma warning( push )

/*   Warnings disabled
*   4702: unreachable code
*/
#pragma warning( disable : 4702)
#pragma warning( disable : 4244)
extern NppData nppData;

namespace PYTHON_PLUGIN_MANAGER
{

	PythonPluginManager::PythonPluginManager(): pythonInitialized_(false)
	{
	}

	std::wstring stringToWstring(const char* utf8Bytes)
	{
		//setup converter
		using convert_type = std::codecvt_utf8<typename std::wstring::value_type>;
		std::wstring_convert<convert_type, typename std::wstring::value_type> converter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		return converter.from_bytes(utf8Bytes);
	}

	void threadRun(HANDLE waitEvent)
	{
		WaitForSingleObject(waitEvent, INFINITE);
		//MessageBox(NULL, _T("Finished!"), _T("I waited..."), 0);
		std::wstring  callback_path = std::wstring(stringToWstring(getenv("PORTABLE_WS_TMP_HOME"))) + std::wstring(stringToWstring("\\npp_py_scripts"));

		IPythonPluginManager& manager = getPythonPluginManager();

		std::wifstream ifs(callback_path);
		ifs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring dummy;
		std::wstring name;
		std::wstring group;
		int index;
		while (ifs  >> index) {
			std::getline(ifs, dummy);
			std::getline(ifs, name);
			std::getline(ifs, group);
			

			manager.register_script(
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(name),
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(group),
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(name)
			);
		}
	}

   void PythonPluginManager::loadScriptsImpl()
	{
		try
		{
	
			PythonScript_Exec pse;
			pse.structVersion = 1;

			HANDLE completeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			pse.completedEvent = completeEvent;
			pse.deliverySuccess = FALSE;
			std::wstring  script_path=  std::wstring(stringToWstring ( getenv("PORTABLE_WS_CORE_HOME") )) + std::wstring(stringToWstring ( "\\plugin\\pynpp\\pythonscript\\pyscripts.py" ));

			pse.script = script_path.c_str();
			pse.flags = PYSCRF_SYNC;


			TCHAR pluginName[] = _T("PythonScript.dll");
			CommunicationInfo commInfo;
			commInfo.internalMsg = PYSCR_EXECSCRIPT;
			commInfo.srcModuleName = _T("NppPluginScriptView.dll");

			commInfo.info = reinterpret_cast<void*>(&pse);

			BOOL delivery = SendMessage(nppData._nppHandle, NPPM_MSGTOPLUGIN, reinterpret_cast<WPARAM>(pluginName), reinterpret_cast<LPARAM>(&commInfo));
			if (!delivery)
			{
				MessageBox(NULL, _T("Python Script not found"), _T("Msg2PluginTester"), 0);
				return;
			}


			if (pse.deliverySuccess)
			{
				// MessageBox(NULL, _T("Delivery Success"), _T("Msg2PluginTester"), 0);
			}
			else
			{
				MessageBox(NULL, _T("Delivery FAILED!"), _T("Msg2PluginTester"), 0);
				return;
			}

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadRun, (LPVOID)completeEvent, 0, NULL);

		}
		catch (...)
		{
			OutputDebugString(L"Exception PythonPluginManager::loadScriptsImpl");
		}
	}
   
	void PythonPluginManager::loadScripts()
	{
		__try
		{
			loadScriptsImpl();
		}
		__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
		{
			//do somethimng
			OutputDebugString(L"Load Scripts Structured Exception");
		}
	}

	void PythonPluginManager::finalizePythonImpl()
	{
		try
		{
			
			pythonInitialized_ = false;
		}
		catch (...)
		{
			OutputDebugString(L"Exception. PythonPluginManager::finalizePythonImpll");
		}
	}
	void PythonPluginManager::finalizePython()
	{
		__try
		{
			finalizePythonImpl();
			pythonInitialized_ = false;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			::MessageBox(NULL, TEXT("Finalize Python Structured Exception"), TEXT("Finalize Python Structured Exception"), MB_OK);
		}
	}

	bool PythonPluginManager::initializePythonImpl()
	{
		if (pythonInitialized_)
		{
			return pythonInitialized_;
		}
		try
		{
			pythonInitialized_ = true;
		}
		catch (... )
		{
			OutputDebugString(L"Exception. PythonPluginManager::initializePythonImpl");

			// do something about
#pragma warning( push )

			/*   Warnings disabled
			*   4706: assignment within conditional expression
			*/
#pragma warning( disable : 4706)
			pythonInitialized_ = false;
#pragma warning( pop )


		}
		return pythonInitialized_;
	}
	bool PythonPluginManager::initializePython()
	{
		if (pythonInitialized_)
		{
			return pythonInitialized_;
		}
		__try
		{
			initializePythonImpl();
			pythonInitialized_ = true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			::MessageBox(NULL, TEXT("Initiaize Python Structured Exception"), TEXT("Initiaize Python Structured Exception"), MB_OK);
			pythonInitialized_=  false;
		}
		return pythonInitialized_;
	}

	void PythonPluginManager::initialize()
	{
		initializePython();
	}

	void PythonPluginManager::finalize()
	{
		OutputDebugString(L"PythonPluginManager::finalize Begin");


		finalizePython();
		OutputDebugString(L"PythonPluginManager::finalize End");
	}
	void PythonPluginManager::reloadScripts()
	{
		if (initializePython())
		{
			loadScripts();
		}
	}

	SCRIPT_MANAGER::IScriptRegistry& PythonPluginManager::getScriptRegistry()
	{
		return SCRIPT_MANAGER::getScriptRegistry();
	}

	void PythonPluginManager::set_event_sink(SCRIPT_MANAGER::IScriptRegistryEventSink* sink)
	{
		SCRIPT_MANAGER::IScriptRegistry& registry = getScriptRegistry();
		registry.SetEventSink(sink);
	}

	void PythonPluginManager::register_script(const std::string& reference,
		const std::string& groupname,
		const std::string& scriptname)
	{
		SCRIPT_MANAGER::IScriptRegistry& registry = getScriptRegistry();
		registry.Add(
			STRING_SUPPORT::std_string_utf_to_utf_std_wstring(groupname),
			STRING_SUPPORT::std_string_utf_to_utf_std_wstring(scriptname),
			reference);
	}

	void PythonPluginManager::python_exec(const std::string& cmd)
	{
		NppScriptEngineNamespace::INppScriptEngine& boostpython = NppScriptEngineNamespace::getNppScriptEngine();
		boostpython.exec_python(cmd);
	}
	void PythonPluginManager::python_exec(const std::wstring& cmd)
	{
		NppScriptEngineNamespace::INppScriptEngine& boostpython = NppScriptEngineNamespace::getNppScriptEngine();
		boostpython.exec_python(cmd);
	}

	void PythonPluginManager::run_python_file(const std::string& filepath)
	{
		NppScriptEngineNamespace::INppScriptEngine& boostpython = NppScriptEngineNamespace::getNppScriptEngine();
		boostpython.run_python_file(filepath);

	}
	void PythonPluginManager::run_python_file(const std::wstring& filepath)
	{
		NppScriptEngineNamespace::INppScriptEngine& boostpython = NppScriptEngineNamespace::getNppScriptEngine();
		boostpython.run_python_file(filepath);
	}

	// SCRIPT_MANAGER::IScriptRunner

	void PythonPluginManager::RunScriptImpl(const STRING_SUPPORT::script_reference_type& name)
	{
		try
		{

			PythonScript_Exec pse;
			pse.structVersion = 1;

			pse.deliverySuccess = FALSE;
			std::string script_char = std::string("import pysergplugin.core; pysergplugin.core.RunScript('") + name + std::string("')");
			std::wstring script = stringToWstring(script_char.c_str());
			pse.script = script.c_str();
			pse.flags = 0;


			TCHAR pluginName[] = _T("PythonScript.dll");
			CommunicationInfo commInfo;
			commInfo.internalMsg = PYSCR_EXECSTATEMENT;
			commInfo.srcModuleName = _T("NppPluginScriptView.dll");

			commInfo.info = reinterpret_cast<void*>(&pse);

			BOOL delivery = SendMessage(nppData._nppHandle, NPPM_MSGTOPLUGIN, reinterpret_cast<WPARAM>(pluginName), reinterpret_cast<LPARAM>(&commInfo));
			if (!delivery)
			{
				MessageBox(NULL, _T("Python Script not found"), _T("Msg2PluginTester"), 0);
				return;
			}


			if (pse.deliverySuccess)
			{
				// MessageBox(NULL, _T("Delivery Success"), _T("Msg2PluginTester"), 0);
			}
			else
			{
				MessageBox(NULL, _T("Delivery FAILED!"), _T("Msg2PluginTester"), 0);
				return;
			}

		}
		catch (...)
		{
			OutputDebugString(L"Exception PythonPluginManager::RunScriptImpl");
		}
	}

	void PythonPluginManager::RunScript(const STRING_SUPPORT::script_reference_type& name)
	{
		try
		{

			OutputDebugString(L"RunScript Started");
			RunScriptImpl(name);
			OutputDebugString(L"RunScript done");

		}
		catch (...)
		{
			OutputDebugString(L"Exception RunScript");
		}
	}


	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager()
	{
		static PythonPluginManager inst;

		return inst;
	}


}

#pragma warning( pop )