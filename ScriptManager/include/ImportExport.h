#ifndef SCRIPT_MANAGER_IMPORTEXPORT_H_GUARD
#define SCRIPT_MANAGER_IMPORTEXPORT_H_GUARD

#ifdef _WIN32
	#ifdef NPP_STANDALONE
			#define SCRIPT_MANAGER_API
	#else
		#ifdef SCRIPT_MANAGER_EXPORTS
			#define SCRIPT_MANAGER_API	__declspec(dllexport)
		#else
			#define SCRIPT_MANAGER_API	__declspec(dllimport)
		#endif
	#endif
#endif


#endif // SCRIPT_MANAGER_IMPORTEXPORT_H_GUARD
