//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#include "stdafx.h"
#include <stdio.h>
#include "NppDockingTemplate/include/StaticDialog.h"
#include "NppScriptWinSupport/include/TreeView.h"
#include "NppScriptWinSupport/include/SampleDialogBox.h"

#include <sstream>
void StaticDialog::goToCenter()
{
	RECT rc;
	::GetClientRect(_hParent, &rc);
	POINT center;
	center.x = rc.left + (rc.right - rc.left)/2;
	center.y = rc.top + (rc.bottom - rc.top)/2;
	::ClientToScreen(_hParent, &center);

	int x = center.x - (_rc.right - _rc.left)/2;
	int y = center.y - (_rc.bottom - _rc.top)/2;

	::SetWindowPos(_hSelf, HWND_TOP, x, y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
}


void StaticDialog::create()
{
	_hSelf = WindowSupport::createSampleDialogBox(_hInst, _hParent);

	if (!_hSelf)
	{
		DWORD err = ::GetLastError();
		std::stringstream stream;
		stream << "CreateDialogParam() return NULL.\rGetLastError() == " << err;
		::MessageBoxA(NULL, stream.str().c_str(), "In StaticDialog::create()", MB_OK);
		return;
	}
	

	// if the destination of message NPPM_MODELESSDIALOG is not its parent, then it's the grand-parent
	::SendMessage(_hParent, NPPM_MODELESSDIALOG, MODELESSDIALOGADD, reinterpret_cast<WPARAM>(_hSelf));

}

INT_PTR CALLBACK StaticDialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			StaticDialog *pStaticDlg = reinterpret_cast<StaticDialog *>(lParam);
			pStaticDlg->_hSelf = hwnd;
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, static_cast<LONG_PTR>(lParam));
			::GetWindowRect(hwnd, &(pStaticDlg->_rc));
			pStaticDlg->run_dlgProc(message, wParam, lParam);

			return TRUE;
		}

		default:
		{
			StaticDialog *pStaticDlg = reinterpret_cast<StaticDialog *>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (!pStaticDlg)
				return FALSE;
			return pStaticDlg->run_dlgProc(message, wParam, lParam);
		}
	}
}

void StaticDialog::alignWith(HWND handle, HWND handle2Align, PosAlign pos, POINT & point)
{
	RECT rc, rc2;
	::GetWindowRect(handle, &rc);

	point.x = rc.left;
	point.y = rc.top;

	switch (pos)
	{
		case PosAlign::left:
		{
			::GetWindowRect(handle2Align, &rc2);
			point.x -= rc2.right - rc2.left;
			break;
		}
		case PosAlign::right:
		{
			::GetWindowRect(handle, &rc2);
			point.x += rc2.right - rc2.left;
			break;
		}
		case PosAlign::top:
		{
			::GetWindowRect(handle2Align, &rc2);
			point.y -= rc2.bottom - rc2.top;
			break;
		}
		case PosAlign::bottom:
		{
			::GetWindowRect(handle, &rc2);
			point.y += rc2.bottom - rc2.top;
			break;
		}
	}

	::ScreenToClient(_hSelf, &point);
}

