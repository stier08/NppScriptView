#include "stdafx.h"

#include "ScriptManager/include/Script.h"
#include "ScriptManager/include/ScriptRegistry.h"
#include "ScriptManager/include/IScriptRegistry.h"
#include "NppPython/include/IPythonPluginManager.h"

#include <exception>
namespace SCRIPT_MANAGER
{
	//////////////////////////////////////////////////////////////////////////
	// Script
	//////////////////////////////////////////////////////////////////////////

	void Script::RunImpl()
	{
		try
		{
			PYTHON_PLUGIN_MANAGER::IPythonPluginManager& manager = PYTHON_PLUGIN_MANAGER::getPythonPluginManager();
			manager.RunScript(scriptReference_);

		}
		catch (const std::exception& ex)
		{
			//do somethimng
			OutputDebugString(L"Exception. Script::RunImpl");
			OutputDebugStringA(ex.what());
		}
	}

	void Script::Run()
	{
		RunImpl();
	}

	const wchar_t* Script::getScriptNameCStrW() const
	{
		return scriptName_.c_str();
	}

	const char* Script::getScriptReferenceCStr() const
	{
		return scriptReference_.c_str();
	}

	const STRING_SUPPORT::script_name_type& Script::getScriptName() const
	{
		return scriptName_;
	}

	const STRING_SUPPORT::script_reference_type& Script::getScriptReference() const
	{
		return scriptReference_;
	}

}
