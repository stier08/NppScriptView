#ifndef BOOST_PYTHON_H__INCLUDED
#define BOOST_PYTHON_H__INCLUDED

#include "NppScriptEngine/include/ImportExport.h"



namespace NppScriptEngineNamespace
{
	class NPP_PYSCRIPT_PYTHON_API INppScriptEngine
	{
	public:
		virtual ~INppScriptEngine() {};
		virtual void exec_python(const std::wstring& command) = 0;
		virtual void exec_python(const std::string& command) = 0;
		virtual void run_python_file(const std::string& filepath) = 0;
		virtual void run_python_file(const std::wstring& filepath) = 0;
		virtual void initialize() = 0;
	};
	NPP_PYSCRIPT_PYTHON_API INppScriptEngine& getNppScriptEngine();
	std::string parse_python_exception();
}

#endif // #ifndef PYTHON_ENGINE_H__INCLUDED