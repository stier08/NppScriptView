#include "stdafx.h"


#include "NppWrapper/include/NppWrapper.h"
//#include "NppScintillaPython/include/NotepadPlusWrapper.h"
//#include "NppScintillaPython/include/GILManager.h"
#include "Common/include/StringSupport.h"

namespace NPP_WRAPPER
{
	class SelectiontextHolder
	{
	public:
		inline explicit SelectiontextHolder(size_t length) :
			m_bufferLen(length + 1),
			m_bufferPtr(new char[m_bufferLen])
		{
			if (m_bufferPtr && m_bufferLen > 0) m_bufferPtr[m_bufferLen - 1] = '\0';
		}
		inline explicit SelectiontextHolder(int length) :
			m_bufferLen(length >= 0 ? (size_t)(length + 1) : 0),
			m_bufferPtr(new char[m_bufferLen])
		{
			if (m_bufferPtr && m_bufferLen > 0) m_bufferPtr[m_bufferLen - 1] = '\0';
		}
		inline explicit SelectiontextHolder(LRESULT length) :
			m_bufferLen(length >= 0 ? (size_t)(length + 1) : 0),
			m_bufferPtr(new char[m_bufferLen])
		{
			if (m_bufferPtr && m_bufferLen > 0) m_bufferPtr[m_bufferLen - 1] = '\0';
		}
		inline ~SelectiontextHolder() { delete[] m_bufferPtr; }
		inline char* operator*() { return m_bufferPtr; }
		inline const char* c_str() const { return m_bufferPtr; }
		inline size_t size() const { return m_bufferLen; }
	private:
		SelectiontextHolder() = delete;  // default constructor disabled
		SelectiontextHolder(const SelectiontextHolder&) = delete; // copy constructor disabled
		SelectiontextHolder& operator = (const SelectiontextHolder&) = delete; // Disable assignment operator disabled
		size_t m_bufferLen;
		char* m_bufferPtr;
	};

	// boost::python::str NotepadPlusWrapper::getCurrentFilename()

	LRESULT NppWrapper::callNotepad(UINT message, WPARAM wParam /*= 0*/, LPARAM lParam/*= 0*/)
	{
		return SendMessage(nppHandle_, message, wParam, lParam);
	}

	LRESULT NppWrapper::callScintilla(UINT message, WPARAM wParam /*= 0*/,LPARAM lParam /*= 0*/)
	{
		int which = -1;
		::SendMessage(nppHandle_, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
		if (-1 == which)
		{
			throw std::runtime_error("Failed to get NPPM_GETCURRENTSCINTILLA");
		}

		HWND curScintillaHandle = (which == 0) ? scintillaMainHandle_ : scintillaSecondHandle_;

		return SendMessage(curScintillaHandle, message, wParam, lParam);
	}


	void NppWrapper::initialize(HINSTANCE hInst, HWND nppHandle,
		HWND scintillaMainHandle,
		HWND scintillaSecondHandle)
	{
		hDllHinst_ = hInst;
		nppHandle_ = nppHandle;

		scintillaMainHandle_ = scintillaMainHandle;
		scintillaSecondHandle_ = scintillaSecondHandle;

	}


	std::wstring NppWrapper::getSelectionTextW()
	{
		return STRING_SUPPORT::std_string_utf_to_utf_std_wstring(getSelectionTextA());
	}

	std::string NppWrapper::getSelectionTextA()
	{
		SelectiontextHolder result(callScintilla(SCI_GETSELTEXT));
		callScintilla(SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(*result));
		return std::string(result.c_str());
	}

	std::wstring NppWrapper::getActiveDocumentFilePathW()
	{

		idx_t bufferID = callNotepad(NPPM_GETCURRENTBUFFERID);
		TCHAR buffer[MAX_PATH];
		callNotepad(NPPM_GETFULLPATHFROMBUFFERID, bufferID, reinterpret_cast<LPARAM>(buffer));
		return std::wstring(buffer);
	}
	
	
	std::string NppWrapper::getActiveDocumentFilePathA()
	{
		return STRING_SUPPORT::std_wstring_utf_to_utf_std_string(getActiveDocumentFilePathW());
	}

	 INppWrapper& getNppWrapper()
	{
		static NppWrapper instance;
		return instance;

	}

}