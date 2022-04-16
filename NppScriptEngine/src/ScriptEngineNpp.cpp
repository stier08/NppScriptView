
#include "stdafx.h"

#include "NppScriptEngine/include/ScriptEngineMacros.h"
#include "NppScriptEngine/include/IScriptEnginePluginManager.h"
#include "NppScriptEngine/include/ScriptEngineNpp.h"

namespace NPP_PYTHON
{

	void RegisterScript(const char*/* scriptref*/, const char* /*groupname*/, const char* /*scriptname*/)
	{
		// 
	}


	void preinitpynpp()
	{
		OutputDebugString(L"[NppScriptEngine] [BEGIN] preinitpynpp");
		
		OutputDebugString(L"[NppScriptEngine] [END] preinitpynpp");
	}

}




