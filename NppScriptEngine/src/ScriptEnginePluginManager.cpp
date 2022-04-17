#include "stdafx.h"

#include "NppScriptEngine/include/ScriptEngine.h"
#include "NppScriptEngine/include/ScriptEnginePluginManager.h"
#include "ScriptManager/include/IScriptRegistry.h"
#include "Common/include/StringSupport.h"


#include "NppScriptWinSupport/include/StackDump.h"

#include "PythonScript/include/NppPythonScript.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <boost/bind/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp> 
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

	void PythonPluginManager::threadRunner()
	{
		std::wstring  callback_path = std::wstring(stringToWstring(getenv("PORTABLE_WS_TMP_HOME"))) 
			+ std::wstring(stringToWstring("\\npp\\npp_py_scripts"));
		for (int i = 0; i < 100; i++)
		{
			if (boost::filesystem::exists(callback_path))
			{
				break;
			}
			else
			{
				boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
			}
			
		}

		if (!boost::filesystem::exists(callback_path))
		{
			return;
		}


		std::wifstream ifs(callback_path);
		ifs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstring dummy;
		std::wstring name;
		std::wstring group;
		std::wstring exec_cmd;
		int index;
		std::getline(ifs, exec_cmd);
		srcipt_exec_cmd_ =  STRING_SUPPORT::std_wstring_utf_to_utf_std_string( exec_cmd );
		while (ifs  >> index) {
			std::getline(ifs, dummy);
			std::getline(ifs, name);
			std::getline(ifs, group);
			

			register_script(
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(name),
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(group),
				STRING_SUPPORT::std_wstring_utf_to_utf_std_string(name)
			);
		}
	}

	void PythonPluginManager::set_srcipt_exec_cmd(const std::string& cmd)
	{
		srcipt_exec_cmd_ = cmd;
	}

   void PythonPluginManager::loadScriptsImpl()
	{
		try
		{
			if (thread_)
			{
				thread_->join();
			}
			thread_ = boost::shared_ptr<boost::thread>( new boost::thread(boost::bind(&PythonPluginManager::threadRunner, this)) );
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

	void PythonPluginManager::register_script(const std::string& reference,
		const std::string& groupname,
		const std::string& scriptname)
	{
		SCRIPT_MANAGER::IScriptRegistry& registry = SCRIPT_MANAGER::getScriptRegistry();
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
			
			std::string script_char = boost::replace_all_copy(srcipt_exec_cmd_, std::string("%s"), name);
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