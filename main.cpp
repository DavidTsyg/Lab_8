#include <windows.h>
#include "resource.h"
#include "stdafx.h""

LRESULT CALLBACK WinFun(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = "The Art Piece"; 
int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR arg, int WinMode)
{
	HWND hwnd; 
	MSG msg;
	WNDCLASSEX wcl; 
					
	wcl.hInstance = hIns;
	wcl.lpszClassName = WinName;
	wcl.lpfnWndProc = WinFun; 
	wcl.style = 0;
	wcl.cbSize = sizeof(WNDCLASSEX);
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wcl.lpszMenuName = NULL; 
	wcl.cbClsExtra = 0; 
	wcl.cbWndExtra = 0;
	
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	if (!RegisterClassEx(&wcl)) return 0;
	
	hwnd = CreateWindow(
		WinName,
		"The Art Piece", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		HWND_DESKTOP, 
		NULL, 
		hIns, 
		NULL); 
			   
	ShowWindow(hwnd, WinMode);
	UpdateWindow(hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


struct Figure
{
	int x1, x2, y1, y2;
};

struct FigureList 
{
	Figure F;
	FigureList *pNext;
};

FigureList *pFirst = 0, *p;

void add(FigureList *&pF, FigureList *p)
{ 
	p->pNext = pF;
	pF = p;
}

int x1, x2, y1, y2;
HPEN pB, pW;
HBRUSH hbrush;
LRESULT CALLBACK WinFun(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		pB = (HPEN)CreatePen(PS_DOT, NULL, RGB(0, 255, 0));

		pW = (HPEN)GetStockObject(WHITE_PEN);

		break;
	case WM_RBUTTONDOWN:
		x1 =  LOWORD(lParam);
		y1 =  HIWORD(lParam);
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_RBUTTON) 
		{
			hdc = GetDC(hwnd);
			hbrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 255, 255));
			SelectObject(hdc, pW);
			SelectObject(hdc, hbrush);
			MoveToEx(hdc, x1, y1, (LPPOINT)NULL);
			Ellipse(hdc, x1, y1, x2, y2);
			x2 = LOWORD(lParam);
			y2 = HIWORD(lParam);
			hbrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 255, 0));
			SelectObject(hdc, pB);
			SelectObject(hdc, hbrush);
			MoveToEx(hdc, x1, y1, (LPPOINT)NULL);
			Ellipse(hdc, x1, y1, x2, y2);
			ReleaseDC(hwnd, hdc);
		}
		break;
	case WM_RBUTTONUP: 
		p = new FigureList;
		p->F.x1 = x1; p->F.x2 = x2;
		p->F.y1 = y1; p->F.y2 = y2;
		add(pFirst, p);
		break;
	case WM_PAINT: 
		hdc = BeginPaint(hwnd, &ps);
		p = pFirst;
		while (p)
		{
			hbrush = CreateHatchBrush(HS_HORIZONTAL, RGB(0, 0, 255));
			SelectObject(hdc, pB);
			SelectObject(hdc, hbrush);
			MoveToEx(hdc, p->F.x1, p->F.y1, (LPPOINT)NULL);
			Ellipse(hdc, p->F.x1, p->F.y1, p->F.x2, p->F.y2);

			p = p->pNext;
		}
		EndPaint(hwnd, &ps);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
