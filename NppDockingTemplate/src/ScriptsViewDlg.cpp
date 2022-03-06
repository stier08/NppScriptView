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
#include "NppDockingTemplate/include/ScriptsViewDlg.h"
#include "NppScriptWinSupport/include/TreeView.h"

extern NppData nppData;

INT_PTR CALLBACK ScriptsViewDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case IDOK :
				{
					return FALSE;
				}
			}
				return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
}

void ScriptsViewDlg::OnScriptAdded(SCRIPT_MANAGER::IScriptGroup* group, SCRIPT_MANAGER::IScript* script)
{
	HWND treeview = ::GetDlgItem(_hSelf, 1);

	WindowSupport::addSriptToTreeView(
		treeview,
		group,
		script
	);
}
void ScriptsViewDlg::OnScriptRemoved(SCRIPT_MANAGER::IScriptGroup* /*group*/, SCRIPT_MANAGER::IScript* /*script*/)
{

}

void ScriptsViewDlg::display(bool toShow /*= true*/) const 
{
	DockingDlgInterface::display(toShow);
}

void ScriptsViewDlg::setParent(HWND parent2set)
{
	_hParent = parent2set;
};