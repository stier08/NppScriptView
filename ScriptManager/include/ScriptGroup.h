#ifndef scriptgroup_h__included
#define scriptgroup_h__included

#include "ScriptManager/include/IScript.h"
#include "ScriptManager/include/IScriptGroup.h"
#include "ScriptManager/include/StringSupport.h"

#include <list>

namespace SCRIPT_MANAGER
{

	/**
	* Named group of scripts
	*/
	class  ScriptGroup : public IScriptGroup
	{
	public:
		explicit ScriptGroup(const STRING_SUPPORT::script_group_name_type& groupName);
		virtual  ~ScriptGroup();

		/**
		* Add a script with a name and a unique reference.
		*/
		virtual IScript* Add(const STRING_SUPPORT::script_name_type& scriptName,
			const STRING_SUPPORT::script_reference_type& scriptReference);

		/**
		* Add a Script instance.
		*/
		virtual  void Add(IScript* script);

		/**
		* Remove a Script instance.
		*/
		virtual  void Remove(IScript* script);

		/**
		* Get a script instance by name, or null if not found.
		*/
		virtual  IScript* Get(const STRING_SUPPORT::script_name_type& scriptName);

		virtual  void Clear();

		virtual  const script_list_t& GetScripts();

		/**
		* Get the name of this group.
		*/
		virtual  const STRING_SUPPORT::script_name_type& GetName() const;
		virtual const wchar_t* GetNameCStrW() const;

	private:
		void clear();

		STRING_SUPPORT::script_group_name_type		groupName_;
		script_list_t	m_scripts;
	};
}

#endif