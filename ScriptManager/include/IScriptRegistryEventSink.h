#ifndef _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED
#define _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED

#include "ScriptManager/include/IScriptGroup.h"
#include "ScriptManager/include/IScript.h"

namespace SCRIPT_MANAGER
{
	class SCRIPT_MANAGER_API IScriptRegistryEventSink
	{
	public:
		virtual void OnScriptAdded(IScriptGroup* group, IScript* script) = 0;
		virtual void OnScriptRemoved(IScriptGroup* group, IScript* script) = 0;
	};

}

#endif // 