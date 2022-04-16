#ifndef iscriptgroup_h__included
#define iscriptgroup_h__included

#include "ScriptManager/include/Script.h"
#include "ScriptManager/include/StringSupport.h"

#include <list>

namespace SCRIPT_MANAGER
{
	typedef std::list<IScript*> script_list_t;

	/**
	* Named group of scripts
	*/
	class SCRIPT_MANAGER_API IScriptGroup
	{
	public:

		virtual ~IScriptGroup() {};

		/**
		* Add a script with a name and a unique reference.
		*/
		virtual IScript* Add(const STRING_SUPPORT::script_name_type& scriptName,
			const STRING_SUPPORT::script_reference_type& scriptReference) = 0;

		/**
		* Add a Script instance.
		*/
		virtual void Add(IScript* script) = 0;

		/**
		* Remove a Script instance.
		*/
		virtual void Remove(IScript* script) = 0;

		/**
		* Get a script instance by name, or null if not found.
		*/
		virtual IScript* Get(const STRING_SUPPORT::script_name_type& scriptName) = 0;

		virtual void Clear() = 0;

		virtual const script_list_t& GetScripts() = 0;

		/**
		* Get the name of this group.
		*/
		virtual const STRING_SUPPORT::script_name_type& GetName() const = 0;
		virtual const wchar_t* GetNameCStrW() const = 0;
	};

}
#endif