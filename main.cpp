#include <iostream>
#include <string>
#include <cmath>
#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);

HWND hwnd;
HWND mainWindow;

HWND hEditPID;

HWND hEditWidth;
HWND hEditHeight;

HWND hEditPosX;
HWND hEditPosY;

BOOL CALLBACK EnumWindowsProcs(HWND hwndloc, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwndloc,&lpdwProcessId);
	if(lpdwProcessId==lParam)
	{
		hwnd = hwndloc;
		return false;
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSA wc = {};

	wc.lpfnWndProc    = WndProc;
	wc.hInstance      = hInstance;
	wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground  = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName  = "MyWinClass";

	if(!RegisterClassA(&wc))
	{
		MessageBoxA(NULL,"call to registerclass failed","exception",0);

		return 0;
	}

	HWND hWnd = CreateWindowA
	(
		"MyWinClass",
		"Resize and move GUI",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		360, 315,
		NULL,
		NULL,
		NULL,
		NULL
	);

	mainWindow = hWnd;

	if(hWnd==NULL)
	{
		MessageBoxA(NULL,"window creation failed","exception",0);

		return 0;
	}

	ShowWindow(hWnd,nShowCmd);

	MSG msg = {};
	while (GetMessageA(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
	case WM_CREATE:
		AddControls(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		if(LOWORD(wParam)==IDC_APPLYBUTTON)
		{
			char PIDchar[32];
			char Widthchar[32];
			char Heightchar[32];
			char PosXchar[32];
			char PosYchar[32];

			int PID;
			int width;
			int height;
			int posX;
			int posY;

			GetWindowTextA(hEditPID,PIDchar,32);
			GetWindowTextA(hEditWidth,Widthchar,32);
			GetWindowTextA(hEditHeight,Heightchar,32);
			GetWindowTextA(hEditPosX,PosXchar,32);
			GetWindowTextA(hEditPosY,PosYchar,32);

			PID = (int)std::stod(PIDchar);
			width = (int)std::stod(Widthchar);
			height = (int)std::stod(Heightchar);
			posX = (int)std::stod(PosXchar);
			posY = (int)std::stod(PosYchar);

			//TEXT BOXES TO NUMBERS

			bool keepPos = false;
			bool keepSize = false;

			keepPos = IsDlgButtonChecked(hWnd,IDC_NOMOVECHECKBOX);
			keepSize = IsDlgButtonChecked(hWnd,IDC_NOSIZECHECKBOX);

			EnumWindows(EnumWindowsProcs,PID);

			if(hwnd==NULL)
			{
				MessageBoxA(NULL,"couldn't find a window with that pid","exception",0);

				return 0;
			}

			SetWindowPos(hwnd, NULL, posX, posY, width+GetSystemMetrics(SM_CXDLGFRAME)*2, height+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYDLGFRAME)*2, (keepPos ? SWP_NOMOVE : 0x0000) | (keepSize ? SWP_NOSIZE : 0x0000) | SWP_NOOWNERZORDER);
			UpdateWindow(hwnd);
		}

		if(LOWORD(wParam)==IDC_LOADBUTTON)
		{
			char PIDchar[32];

			int PID;

			GetWindowTextA(hEditPID,PIDchar,32);

			PID = (int)std::stod(PIDchar);

			EnumWindows(EnumWindowsProcs,PID);

			if(hwnd==NULL)
			{
				MessageBoxA(NULL,"couldn't find a window with that pid","exception",0);

				return 0;
			}

			RECT rect = {NULL};
			if(GetWindowRect(hwnd, &rect )) 
			{
				SetWindowTextA(hEditWidth,std::to_string(rect.right-rect.left-GetSystemMetrics(SM_CXDLGFRAME)*2).c_str());
				SetWindowTextA(hEditHeight,std::to_string(rect.bottom-rect.top-GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYDLGFRAME)*2).c_str());
				SetWindowTextA(hEditPosX,std::to_string(rect.left).c_str());
				SetWindowTextA(hEditPosY,std::to_string(rect.top).c_str());
			}
		}

		if(LOWORD(wParam)==IDC_NOMOVECHECKBOX)
		{
			if(IsDlgButtonChecked(hWnd,IDC_NOMOVECHECKBOX))
			{
				CheckDlgButton(hWnd,IDC_NOMOVECHECKBOX,BST_UNCHECKED);
			} else
			{
				CheckDlgButton(hWnd,IDC_NOMOVECHECKBOX,BST_CHECKED);
			}
		}

		if(LOWORD(wParam)==IDC_NOSIZECHECKBOX)
		{
			if(IsDlgButtonChecked(hWnd,IDC_NOSIZECHECKBOX))
			{
				CheckDlgButton(hWnd,IDC_NOSIZECHECKBOX,BST_UNCHECKED);
			} else
			{
				CheckDlgButton(hWnd,IDC_NOSIZECHECKBOX,BST_CHECKED);
			}
		}

		break;

	default:
		return DefWindowProcA(hWnd, message, wParam, lParam);
	}

	return 0;
}

void AddControls(HWND hWnd)
{
	CreateWindowA("static","PID of the window",WS_VISIBLE|WS_CHILD|ES_CENTER,15,15,150,25,hWnd,NULL,NULL,NULL);
	hEditPID = CreateWindowExA(WS_EX_CLIENTEDGE,"edit","",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,15,40,150,25,hWnd,NULL,NULL,NULL);


	CreateWindowA("static","new width of window",WS_VISIBLE|WS_CHILD|ES_CENTER,15,70,150,25,hWnd,NULL,NULL,NULL);
	hEditWidth = CreateWindowExA(WS_EX_CLIENTEDGE,"edit","0",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,15,95,150,25,hWnd,NULL,NULL,NULL);

	CreateWindowA("static","new height of window",WS_VISIBLE|WS_CHILD|ES_CENTER,175,70,150,25,hWnd,NULL,NULL,NULL);
	hEditHeight = CreateWindowExA(WS_EX_CLIENTEDGE,"edit","0",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,175,95,150,25,hWnd,NULL,NULL,NULL);


	CreateWindowA("static","new x pos of window",WS_VISIBLE|WS_CHILD|ES_CENTER,15,125,150,25,hWnd,NULL,NULL,NULL);
	hEditPosX = CreateWindowExA(WS_EX_CLIENTEDGE,"edit","0",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,15,150,150,25,hWnd,NULL,NULL,NULL);

	CreateWindowA("static","new y pos of window",WS_VISIBLE|WS_CHILD|ES_CENTER,175,125,150,25,hWnd,NULL,NULL,NULL);
	hEditPosY = CreateWindowExA(WS_EX_CLIENTEDGE,"edit","0",WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL,175,150,150,25,hWnd,NULL,NULL,NULL);


	CreateWindowA("button","no move",WS_VISIBLE|WS_CHILD|BS_CHECKBOX,15,185,150,25,hWnd,(HMENU)IDC_NOMOVECHECKBOX,NULL,NULL);

	CreateWindowA("button","no size",WS_VISIBLE|WS_CHILD|BS_CHECKBOX,175,185,150,25,hWnd,(HMENU)IDC_NOSIZECHECKBOX,NULL,NULL);


	CreateWindowA("button","load",WS_VISIBLE|WS_CHILD,175,225,150,35,hWnd,(HMENU)IDC_LOADBUTTON,NULL,NULL);

	CreateWindowA("button","apply",WS_VISIBLE|WS_CHILD|WS_BORDER,15,225,150,35,hWnd,(HMENU)IDC_APPLYBUTTON,NULL,NULL);
}