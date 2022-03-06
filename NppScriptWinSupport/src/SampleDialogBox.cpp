#include "stdafx.h"

#include <NppPyScriptWinSupport/include/SampleDialogBox.h>
#include <NppPyScriptWinSupport/include/TreeView.h>
#include <cstdint>
#include <cassert>

#include <windows.h>


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

void CreateDialogBox(HWND);
void RegisterDialogClass(HWND);

static HINSTANCE g_hInstance;

LRESULT CALLBACK SampleWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
	{
		RegisterDialogClass(hwnd);
		WindowSupport::createSampleTreeView(g_hInstance, hwnd);
		//::SetWindowLong(hwnd, GWL_USERDATA, (LONG)treeview);
	}
		break;
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT	rc = { 0 };
		//HWND treeview = (HWND)(::GetWindowLong(hwnd, GWL_USERDATA));
		HWND treeview = ::GetDlgItem(hwnd, 1);
		::GetClientRect(hwnd, &rc);
		::SetWindowPos(treeview, NULL, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	break;
	case WM_COMMAND:
		//CreateDialogBox(hwnd);
		break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CREATE:
		break;

	case WM_COMMAND:
		DestroyWindow(hwnd);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	}

	return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

void  RegisterDialogClass(HWND /*hwnd*/) {

	WNDCLASSEXW wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = (WNDPROC)DialogProc;
	wc.hInstance = g_hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = L"DialogClass";
	RegisterClassExW(&wc);

}

void CreateDialogBox(HWND hWndParent) 
{
  // https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles
  // https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles
	CreateWindowExW(
    WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, 
    L"DialogClass", 
    L"Dialog Box",
		WS_CHILD | WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
    100, 100, 200, 150,
		hWndParent,  /*hWndParent*/
    NULL,  /*hMenu*/
    g_hInstance,
 NULL);
}

namespace WindowSupport
{
	NPP_PYSCRIPT_WIN_SUPPORT_API HWND createSampleDialogBox(HINSTANCE hInstance, HWND hParent)
	{
   // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindoww
		HWND hwnd;
		g_hInstance = hInstance;
		WNDCLASSW wc = { 0 };

		wc.lpszClassName = L"Window";
		wc.hInstance = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc = SampleWndProc;

		RegisterClassW(&wc);
		hwnd = CreateWindowW(wc.lpszClassName, L"Scripts",
			WS_CHILD | WS_THICKFRAME  | WS_SYSMENU | WS_CAPTION | WS_VISIBLE,
			100, 100, 250, 150, 
      hParent,  /*hWndParent*/
      NULL, /*hMenu*/
      hInstance, 
      NULL);
		return hwnd;
	}

}