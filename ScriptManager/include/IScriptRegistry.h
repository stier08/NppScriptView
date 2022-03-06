#ifndef _iscriptregistry_h__included
#define _iscriptregistry_h__included

#include "ScriptManager/include/IScriptRunner.h"
#include "ScriptManager/include/StringSupport.h"
#include "ScriptManager/include/ScriptGroup.h"
#include "ScriptManager/include/IScriptRegistryEventSink.h"


namespace SCRIPT_MANAGER
{
	typedef std::list<IScriptGroup*> group_list_t;
	/**
	* @brief The Script Registry
	*
	* Interface for the script registry. The script registry maps scripts to
	* runners (@see IScriptRunner) using a unique identifier (e.g. "python", "tcl").
	*/
	class SCRIPT_MANAGER_API IScriptRegistry
	{
	public:
		virtual ~IScriptRegistry() {}

		/**
		* Add a named script to the registry.
		* @param group Name of a group to insert the script in
		* @param name Friendly name for the script
		* @param scriptref Reference for the script, in the form "runnerId:scriptId"
		*/
		virtual void Add(const STRING_SUPPORT::script_group_name_type& group,
			const STRING_SUPPORT::script_name_type& name,
			const STRING_SUPPORT::script_reference_type& scriptref) = 0;

		/**
		* Register a script runner using a unique runner ID
		*/
		virtual void RegisterRunner(const STRING_SUPPORT::script_reference_type& id, IScriptRunner* runner) = 0;

		/**
		* Remove a runner by ID
		*/
		virtual void RemoveRunner(const STRING_SUPPORT::script_reference_type&id) = 0;

		/**
		* Get a runner for a given ID
		*/
		virtual IScriptRunner* GetRunner(const STRING_SUPPORT::script_reference_type& id) = 0;

		/**
		* Enable scripts for a given scheme id. The runner id is used by PN to
		* find the right runner to run the script.
		*
		* e.g. EnableSchemeScripts("python", "python") means that PN allows the
		* user to set python files as scripts at run-time.
		*/
		virtual void EnableSchemeScripts(const char* scheme, const STRING_SUPPORT::script_reference_type& runnerId) = 0;


		virtual void SetEventSink(IScriptRegistryEventSink* sink) = 0;
		virtual const group_list_t& GetGroups() = 0;
	};

	SCRIPT_MANAGER_API IScriptRegistry& getScriptRegistry();
}


#endif