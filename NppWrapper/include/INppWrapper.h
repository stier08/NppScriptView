#ifndef INPPWRAPPER_H__INCLUDED
#define INPPWRAPPER_H__INCLUDED

#include "NppWrapper/include/ImportExport.h"

namespace NPP_WRAPPER
{
	class NPP_WRAPPER_API INppWrapper
	{
	public:
		virtual ~INppWrapper() {};
		virtual void initialize(HINSTANCE hInst, 
			HWND nppHandle,
			HWND scintillaMainHandle,
			HWND scintillaSecondHandle
			) = 0 ;

		virtual std::wstring getSelectionTextW() = 0;
		virtual std::string getSelectionTextA() = 0;

		virtual std::wstring getActiveDocumentFilePathW() = 0;
		virtual std::string getActiveDocumentFilePathA() = 0;
	};
	NPP_WRAPPER_API INppWrapper& getNppWrapper();
}

#endif // #ifndef INPPWRAPPER_H__INCLUDED