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

#ifndef SCRIPTS_VIEW_DLG_H
#define SCRIPTS_VIEW_DLG_H
#include "NppDockingTemplate/include/DockingDlgInterface.h"
#include "NppDockingTemplate/include/ImportExport.h"
#include "ScriptManager/include/IScriptRegistryEventSink.h"
#include "resource.h"

class NPP_SCRIPTVIEW_DOCKING_API ScriptsViewDlg : 
		public DockingDlgInterface, 
	public SCRIPT_MANAGER::IScriptRegistryEventSink

{
public :
	ScriptsViewDlg() : DockingDlgInterface(IDD_PLUGINGOLINE_DEMO){};

	virtual void display(bool toShow = true) const;

	void setParent(HWND parent2set);
	void register_script(const std::string& reference,
		const std::string& groupname,
		const std::string& scriptname);

public:
	/*
	IScriptRegistryEventSink
	*/
	virtual void OnScriptAdded(SCRIPT_MANAGER::IScriptGroup* group, SCRIPT_MANAGER::IScript* script);
	virtual void OnScriptRemoved(SCRIPT_MANAGER::IScriptGroup* group, SCRIPT_MANAGER::IScript* script);

protected :
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);


};

#endif //SCRIPTS_VIEW_DLG_H
