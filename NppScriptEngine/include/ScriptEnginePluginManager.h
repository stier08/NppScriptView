#ifndef PYTHONPLUGINMANAGER_H__INCLUDED
#define PYTHONPLUGINMANAGER_H__INCLUDED

#include "NppScriptEngine/include/ImportExport.h"
#include "NppScriptEngine/include/IScriptEnginePluginManager.h"
#include  <boost/thread.hpp>
#include  <boost/shared_ptr.hpp>

namespace PYTHON_PLUGIN_MANAGER
{
	class  PythonPluginManager :public boost::noncopyable, public IPythonPluginManager
	{
	private:

		bool pythonInitialized_;
		std::string srcipt_exec_cmd_;
		boost::shared_ptr<boost::thread> thread_;

		void finalizePythonImpl();
		void finalizePython();

		bool initializePythonImpl();
		bool initializePython();
		void loadScriptsImpl();
		void loadScripts();
		void RunScriptImpl(const STRING_SUPPORT::script_reference_type& name);
		void threadRunner();
	public:
		PythonPluginManager();
		virtual void initialize();
		virtual void finalize();
		virtual void reloadScripts();

		virtual void register_script(const std::string& reference,
			const std::string& groupname,
			const std::string& scriptname);

		virtual void python_exec(const std::string& cmd) ;
		virtual void python_exec(const std::wstring& cmd);

		virtual void run_python_file(const std::string& filepath);
		virtual void run_python_file(const std::wstring& filepath);

		// SCRIPT_MANAGER::IScriptRunner
		virtual void RunScript(const STRING_SUPPORT::script_reference_type& name);
		// 
		void set_srcipt_exec_cmd(const std::string& srcipt_exec_cmd);
	};

}

#endif // #ifndef PYTHONPLUGINMANAGER_H__INCLUDED

