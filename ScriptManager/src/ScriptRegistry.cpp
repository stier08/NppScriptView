#include "stdafx.h"
#include "ScriptManager/include/ScriptRegistry.h"
#include "ScriptManager/include/IScriptRunner.h"
#include "ScriptManager/include/Script.h"
#include "ScriptManager/include/ScriptGroup.h"

#include <boost/noncopyable.hpp>
#include <map>
#include <list>
#include <string>

namespace SCRIPT_MANAGER
{
	ScriptRegistry::ScriptRegistry()
	{
		m_sink = NULL;
	}

	ScriptRegistry::~ScriptRegistry()
	{
		clear();
	}

	void ScriptRegistry::Add(const STRING_SUPPORT::script_group_name_type& groupName, Script* script)
	{
		IScriptGroup* pGroup = getOrMakeGroup(groupName);
		pGroup->Add(script);

		if (m_sink)
			m_sink->OnScriptAdded(pGroup, script);
	}

	void ScriptRegistry::Remove(const STRING_SUPPORT::script_group_name_type& groupName, Script* script)
	{
		IScriptGroup* pGroup = getOrMakeGroup(groupName);
		pGroup->Remove(script);

		if (m_sink)
			m_sink->OnScriptRemoved(pGroup, script);
	}

	void ScriptRegistry::Add(const STRING_SUPPORT::script_group_name_type& groupName,
		const STRING_SUPPORT::script_name_type& scriptName,
		const STRING_SUPPORT::script_reference_type& scriptReference)
	{
		IScriptGroup* pGroup = getOrMakeGroup(groupName);
		IScript* theScript = pGroup->Add(scriptName, scriptReference);

		if (m_sink)
			m_sink->OnScriptAdded(pGroup, theScript);
	}

	IScript* ScriptRegistry::FindScript(const STRING_SUPPORT::script_group_name_type& groupName,
		const STRING_SUPPORT::script_name_type& scriptName)
	{
		IScriptGroup* pGroup = getGroup(groupName);
		if (!pGroup)
			return NULL;

		return pGroup->Get(scriptName);
	}

	IScriptGroup* ScriptRegistry::getOrMakeGroup(const STRING_SUPPORT::script_group_name_type& groupName)
	{
		IScriptGroup* pGroup(getGroup(groupName));

		if (!pGroup)
		{
			pGroup = new ScriptGroup(groupName);
			m_groups.push_back(pGroup);
		}

		return pGroup;
	}

	IScriptGroup* ScriptRegistry::getGroup(const STRING_SUPPORT::script_group_name_type& groupName)
	{
		for (group_list_t::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
		{
			if ((*i)->GetName() == groupName)
			{
				return (*i);
			}
		}

		return NULL;
	}

	void ScriptRegistry::Clear()
	{
		clear();
	}

	const group_list_t& ScriptRegistry::GetGroups()
	{
		return m_groups;
	}

	void ScriptRegistry::RegisterRunner(const STRING_SUPPORT::script_reference_type& id, IScriptRunner* runner)
	{
		m_runners.insert(s_runner_map::value_type(std::string(id), runner));
	}

	void ScriptRegistry::RemoveRunner(const STRING_SUPPORT::script_reference_type& id)
	{
		s_runner_map::iterator i = m_runners.find(std::string(id));
		if (i != m_runners.end())
			m_runners.erase(i);
	}

	IScriptRunner* ScriptRegistry::GetRunner(const STRING_SUPPORT::script_reference_type& id)
	{
		s_runner_map::const_iterator i = m_runners.find(std::string(id));
		if (i != m_runners.end())
		{
			return (*i).second;
		}
		else
			return NULL;
	}

	void ScriptRegistry::EnableSchemeScripts(const char* scheme, const STRING_SUPPORT::script_reference_type& runnerId)
	{
		m_scriptableSchemes.insert(string_map::value_type(std::string(scheme), std::string(runnerId)));
	}

	bool ScriptRegistry::SchemeScriptsEnabled(const char* scheme)
	{
		return m_scriptableSchemes.find(std::string(scheme)) != m_scriptableSchemes.end();
	}

	bool ScriptRegistry::SchemeScriptsEnabled(const char* scheme, std::string& runner)
	{
		string_map::const_iterator i = m_scriptableSchemes.find(std::string(scheme));
		if (i != m_scriptableSchemes.end())
		{
			runner = (*i).second;
			return true;
		}
		return false;
	}

	void ScriptRegistry::SetEventSink(IScriptRegistryEventSink* sink)
	{
		m_sink = sink;
	}

	void ScriptRegistry::clear()
	{
		for (group_list_t::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
		{
			delete (*i);
		}

		m_groups.clear();
	}

	IScriptRegistry& getScriptRegistry()
	{
		static ScriptRegistry instance;
		return instance;
	}
}
