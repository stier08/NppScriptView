#ifndef NPP_PWRAPPER_IMPORTEXPORT_H_GUARD
#define NPP_PWRAPPER_IMPORTEXPORT_H_GUARD

#ifdef _WIN32
	#ifdef NPP_STANDALONE
			#define NPP_WRAPPER_API
	#else
		#ifdef NPP_WRAPPER_EXPORTS
			#define NPP_WRAPPER_API	__declspec(dllexport)
		#else
			#define NPP_WRAPPER_API	__declspec(dllimport)
		#endif
	#endif
#endif


#endif // NPP_PWRAPPER_IMPORTEXPORT_H_GUARD
