#ifndef iscript_h__included
#define iscript_h__included

#include <string>
#include "ScriptManager/include/StringSupport.h"

namespace SCRIPT_MANAGER
{
	/**
	* Reference to a single script.
	*/

	class SCRIPT_MANAGER_API IScript
	{
	public:
		virtual ~IScript() {};
		virtual const STRING_SUPPORT::script_name_type& getScriptName() const = 0;
		virtual const STRING_SUPPORT::script_reference_type& getScriptReference() const = 0;

		virtual const wchar_t* getScriptNameCStrW() const = 0;
		virtual const char* getScriptReferenceCStr() const = 0;

		virtual void Run() = 0;
	};
}



#endif // script_h__included