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

	
	void NppScriptEngine::run_python_file(const std::string& /*filepath*/)
	{
		// 

	}

	void NppScriptEngine::run_python_file(const std::wstring& /*filepath*/)
	{
		//
	}

	void NppScriptEngine::exec_python(const std::wstring& /*command*/)
	{
		//
	}

	void NppScriptEngine::exec_python(const std::string& /*command*/)
	{
		// 
	}

	void NppScriptEngine::python_intialize()
	{
		//
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