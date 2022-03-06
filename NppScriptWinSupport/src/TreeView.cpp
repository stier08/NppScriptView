#include "stdafx.h"
#include "CommCtrl.h"
#include "windowsx.h"
#include <NppPyScriptWinSupport/include/TreeView.h>
#include "ScriptManager/include/IScript.h"
#include "ScriptManager/include/IScriptGroup.h"

#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

namespace WindowSupport
{
	HTREEITEM GetChildItem(HWND hwndTree, HTREEITEM node)
	{

		HTREEITEM  childItem = TreeView_GetChild(
			hwndTree,
			node
		);
		return childItem;
	}

	HTREEITEM GetRootItem(HWND hwndTree)
	{
		HTREEITEM rootItem =  TreeView_GetRoot(hwndTree);
		return rootItem;
	}

	std::wstring GetItemText(HWND hwndTree, HTREEITEM node)
	{
		TVITEM tv = { 0 };
		TCHAR textbuf[256];

		tv.mask = TVIF_TEXT | TVIF_HANDLE;
		tv.hItem = node;
		tv.pszText = &textbuf[0];
		tv.cchTextMax =  256;


		 TreeView_GetItem(
			 hwndTree,
			 &tv);
		 return std::wstring(&textbuf[0]);
	}

	HTREEITEM GetNextSiblingItem(HWND hwndTree, HTREEITEM node)
	{
		HTREEITEM  siblingItem = TreeView_GetNextSibling(
			hwndTree,
			node
		);
		return siblingItem;
	}

	HTREEITEM findChild(HWND hwndTree, HTREEITEM root, const std::wstring& name)
	{
		HTREEITEM node;
		if (root != NULL)
		{
			node = GetChildItem(hwndTree, root);
		}
		else
		{
			node = GetRootItem(hwndTree);
		}
		while (node)
		{
			std::wstring  nodeName = GetItemText(hwndTree, node);
			if (name == nodeName)
			{
				return node;
			}
			node = GetNextSiblingItem(hwndTree, node);
		}

		return NULL;
	}

	HTREEITEM findGroup(HWND hwndTree, const std::wstring& name)
	{
		std::wstring  wname(name);

		boost::char_separator<wchar_t> sep(L"/");
		boost::tokenizer<boost::char_separator<wchar_t>,
			std::wstring::const_iterator, std::wstring>  tokens(wname, sep);

		HTREEITEM root = NULL;

		BOOST_FOREACH(std::wstring const& token, tokens)
		{
			root = findChild(hwndTree, root, token);
			if (root == NULL)
			{
				break;
			}
		}
		return root;
	}

	HTREEITEM addGroup(HWND hwndTree,  const std::wstring& name)
	{
		std::wstring  wname(name);

		boost::char_separator<wchar_t> sep(L"/");
		boost::tokenizer<boost::char_separator<wchar_t>,
			std::wstring::const_iterator, std::wstring>  tokens(wname, sep);

		HTREEITEM group = NULL;

		BOOST_FOREACH(const std::wstring & token, tokens)
		{
			HTREEITEM child = findChild(hwndTree, group, token);
			if (child == NULL)
			{
				TVINSERTSTRUCT  data;
				data.hParent = group;
				data.itemex.mask = TVIF_TEXT ;
				data.itemex.pszText = const_cast<LPWSTR>(token.c_str());
				data.itemex.cchTextMax = static_cast<int>(token.size());
				group = TreeView_InsertItem(
					hwndTree,
					&data);
			}
			else
			{
				group = child;
			}
		}
		return group;
	}


	HTREEITEM AddItemToTree(HWND hwndTree, HTREEITEM parent, SCRIPT_MANAGER::IScript* script)
	{
		// https://docs.microsoft.com/en-us/windows/win32/controls/add-tree-view-items
		TVINSERTSTRUCT  data;
		data.hParent = parent;
		data.itemex.mask = TVIF_TEXT | TVIF_PARAM ;
		data.itemex.pszText = const_cast<LPWSTR>(script->getScriptName().c_str());
		data.itemex.cchTextMax = static_cast<int>( script->getScriptName().size() );
		
		data.itemex.lParam = reinterpret_cast<LPARAM>(script);

		HTREEITEM ret = TreeView_InsertItem(
			hwndTree,
			&data);
		return ret;

	}


	// https://docs.microsoft.com/en-us/windows/win32/controls/create-a-tree-view-control

	/*
	CreateWindowExW(
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
	_In_opt_ LPCWSTR lpWindowName,
	_In_ DWORD dwStyle,
	_In_ int X,
	_In_ int Y,
	_In_ int nWidth,
	_In_ int nHeight,
	_In_opt_ HWND hWndParent,
	_In_opt_ HMENU hMenu,
	_In_opt_ HINSTANCE hInstance,
	_In_opt_ LPVOID lpParam
	*/

}
namespace WindowSupport
{
	WNDPROC g_wpOrigEditProc;
	HWND g_hwndTree;
	LRESULT APIENTRY EditSubclassProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam)
	{
		// https://docs.microsoft.com/en-us/windows/win32/controls/bumper-tree-view-control-reference-messages

		if (WM_LBUTTONDBLCLK == uMsg)
		{
			if (hwnd == g_hwndTree)
			{

				POINT			pt = { 0 };
				TVHITTESTINFO	ht = { 0 };
				DWORD			dwpos = ::GetMessagePos();
				HTREEITEM		hItem = NULL;

				pt.x = GET_X_LPARAM(dwpos);
				pt.y = GET_Y_LPARAM(dwpos);

				ht.pt = pt;
				::ScreenToClient(g_hwndTree, &ht.pt);

				hItem = TreeView_HitTest(g_hwndTree, &ht);
				if (hItem != NULL)
				{
						
					TVITEM tv = { 0 };
					tv.mask = TVIF_HANDLE | TVIF_PARAM;
					tv.hItem = hItem;
					TreeView_GetItem(
						g_hwndTree,
						&tv);

					SCRIPT_MANAGER::IScript * script= reinterpret_cast<SCRIPT_MANAGER::IScript*>(tv.lParam);
					if (script)
					{
						OutputDebugString(L"script->Run()");
						script->Run();
						return TRUE;
					}
				}

		
			}
		}


		return CallWindowProc(g_wpOrigEditProc, hwnd, uMsg,
			wParam, lParam);

	}

	NPP_PYSCRIPT_WIN_SUPPORT_API HWND createSampleTreeView(HINSTANCE hInstance, HWND hParent)
	{
		RECT rc;
		GetClientRect(hParent, &rc);

		g_hwndTree = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_TREEVIEW,
			L"TreeView",
			WS_CHILD | WS_VISIBLE,
			0, 0, rc.right, rc.bottom,
			hParent, 
			(HMENU)1,  /*control identifier*/
			hInstance, 
			NULL);
		g_wpOrigEditProc = (WNDPROC)SetWindowLongPtr(g_hwndTree,
			GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);

		return g_hwndTree;
	}


	NPP_PYSCRIPT_WIN_SUPPORT_API HWND addSriptToTreeView(
		HWND hwndTree,
		SCRIPT_MANAGER::IScriptGroup* scriptgroup,
		SCRIPT_MANAGER::IScript* script
	)
	{

		HTREEITEM group = findGroup (hwndTree, scriptgroup->GetName());
		if (!group)
		{
			group = addGroup(hwndTree, scriptgroup->GetName());
		}
		AddItemToTree(hwndTree, group, script);

		return hwndTree;
	}

}

