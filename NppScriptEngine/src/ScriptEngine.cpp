#include "stdafx.h"
#include "NppScriptEngine/include/ScriptEngine.h"
#include "ScriptManager/include/StringSupport.h"


#include  <fstream>
#include  <sstream>

namespace NppScriptEngineNamespace
{

	class NppScriptEngine : public INppScriptEngine
	{
	void python_intialize();
	public:
		virtual void exec_python(const std::wstring& command);
		virtual void exec_python(const std::string& command);
		virtual void run_python_file(const std::string& filepath);
		virtual void run_python_file(const std::wstring& filepath);

		virtual void initialize() ;


	};

	
	void NppScriptEngine::run_python_file(const std::string& filepath)
	{
		boost::python::object result;

		try
		{
			NppPythonScript::GILLock  lock;
			boost::python::object main = boost::python::import("__main__");

			// Retrieve the main module's namespace
			boost::python::object global(main.attr("__dict__"));

			result = boost::python::exec_file(
				filepath.c_str(),
				global, global);
		}

		catch (boost::python::error_already_set&)
		{
			OutputDebugString(L"Exception. BoostPython::run_python_file");
			std::string what = parse_python_exception();
			OutputDebugStringA(what.c_str());
		}

		return result;

	}

	void NppScriptEngine::run_python_file(const std::wstring& filepath)
	{
		return run_python_file(STRING_SUPPORT::std_wstring_utf_to_utf_std_string(filepath));
	}

	void NppScriptEngine::exec_python(const std::wstring& command)
	{
		return exec_python(STRING_SUPPORT::std_wstring_utf_to_utf_std_string(command));
	}

	void NppScriptEngine::exec_python(const std::string& command)
	{
		boost::python::object result;

		try
		{

			NppPythonScript::GILLock  lock;
			boost::python::object main = boost::python::import("__main__");

			// Retrieve the main module's namespace
			boost::python::object global(main.attr("__dict__"));

			result = boost::python::exec(
				command.c_str(),
				global, global);
		}

		catch (boost::python::error_already_set&)
		{
			OutputDebugString(L"Python Exception. BoostPython::exec_python");
			std::string what = parse_python_exception();
			OutputDebugStringA(what.c_str());
		}
		return result;
	}

	void NppScriptEngine::python_intialize()
	{
		if (!Py_IsInitialized())
		{
			Py_Initialize();
		}
		
	}

	void NppScriptEngine::initialize()
	{
		python_intialize();
	}

	NPP_PYSCRIPT_PYTHON_API INppScriptEngine& getNppScriptEngine()

	{
		static NppScriptEngine engine;
		return  engine;
	}
}