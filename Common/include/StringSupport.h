#ifndef string_support_h__included
#define string_support_h__included

#include <string>
#include <exception>
#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <boost/locale.hpp>
#include <boost/locale/conversion.hpp>

#include "ScriptManager/include/ImportExport.h"

namespace STRING_SUPPORT
{ 
	typedef std::wstring script_group_name_type;
	typedef std::wstring script_name_type;
	typedef std::string script_reference_type;
	SCRIPT_MANAGER_API std::string std_wstring_utf_to_utf_std_string(const std::wstring& val);
	SCRIPT_MANAGER_API std::wstring std_string_utf_to_utf_std_wstring(const std::string& val);
	SCRIPT_MANAGER_API CString script_group_name_type_CString(const script_group_name_type& val);
	SCRIPT_MANAGER_API CString script_name_type_CString(const script_name_type& val);
	SCRIPT_MANAGER_API CString script_reference_type_CString(const script_reference_type& val);

}
#endif // string_support_h__included
