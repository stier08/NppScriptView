#ifndef _scriptrunner_h__included
#define _scriptrunner_h__included

#include "ScriptManager/include/StringSupport.h"
#include "ScriptManager/include/IScriptRunner.h"

namespace SCRIPT_MANAGER
{

	/**
	* @brief Script Runner Interface - for Script Engine Implementors
	*
	* Interface for something that can run scripts from the
	* script manager, this is how PN tells you to run a script whether
	* it be a file or a document.
	*
	* It also provides methods allowing a scripting provider to let scripting
	* be used in places like parameter lists.
	*/
	class ScriptRunner: public IScriptRunner
	{
	public:
		virtual ~IScriptRunner() {}

		/**
		* This method requests that a runner runs a named
		* script that it has previously registered with the
		* registry.
		*/
		virtual void RunScript(const STRING_SUPPORT::script_reference_type& name);

	};
}

#endif