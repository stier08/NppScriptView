#ifndef script_h__included
#define script_h__included

#include <string>
#include "ScriptManager/include/StringSupport.h"
#include "ScriptManager/include/IScript.h"

namespace SCRIPT_MANAGER
{
	/**
	* Reference to a single script.
	*/

	class  Script : public IScript
	{
	public:
		virtual ~Script() {}
		explicit Script(const STRING_SUPPORT::script_name_type& scriptName,
			const STRING_SUPPORT::script_reference_type& scriptReference) : scriptName_(scriptName), scriptReference_(scriptReference) {}

		explicit Script(const Script& copy)
		{
			*this = copy;
		}

		Script& operator = (const Script& copy)
		{
			scriptReference_ = copy.scriptReference_;
			scriptName_ = copy.scriptName_;
			return *this;
		}

		virtual const STRING_SUPPORT::script_name_type& getScriptName() const;
		virtual const wchar_t* getScriptNameCStrW() const;

		virtual const STRING_SUPPORT::script_reference_type& getScriptReference() const;
		virtual const char * getScriptReferenceCStr() const;

		virtual void Run();
	private:
		void RunImpl();

		STRING_SUPPORT::script_name_type scriptName_;
		STRING_SUPPORT::script_reference_type scriptReference_;

	};


}
#endif // script_h__included