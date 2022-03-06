#ifndef NPPWRAPPER_H__INCLUDED
#define NPPWRAPPER_H__INCLUDED

#include "NppWrapper/include/ImportExport.h"
#include "NppWrapper/include/INppWrapper.h"



namespace NPP_WRAPPER
{
	class  NppWrapper : public INppWrapper
	{
	private:
		LRESULT callNotepad(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
		LRESULT callScintilla(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

	public:
		virtual ~NppWrapper() {};
		virtual void initialize(HINSTANCE hInst, HWND nppHandle,
			HWND scintillaMainHandle,
			HWND scintillaSecondHandle) ;


		virtual std::wstring getSelectionTextW();
		virtual std::string getSelectionTextA();

		virtual std::wstring getActiveDocumentFilePathW();
		virtual std::string getActiveDocumentFilePathA() ;
	private:
		HINSTANCE hDllHinst_;
		HWND nppHandle_;
		HWND scintillaMainHandle_;
		HWND scintillaSecondHandle_;
	};

}

#endif // #ifndef NPPWRAPPER_H__INCLUDED

