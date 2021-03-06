#pragma once

#include "resource.h"
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <CommCtrl.h>
#include <vector>
#include <string.h>
#include <commctrl.h>
#pragma comment( lib, "comctl32.lib") 
#define NUM_TBBUTTONS                   3
#define NUM_TBBITMAPS                   3
#define BMP_CX                          10
#define BMP_CY                          30

TBBUTTON tbButtons[] = {
	{ 0, ID_TOP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, ID_TOP },
{ 1, ID_RIB, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, ID_RIB },
{ 2, ID_GET, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, ID_GET }
};

struct Vertex {
	int num; // Номер вершины
	HWND hWnd; // описатель
	POINT pos; // позиция
};

struct Rib {
	POINT ver;
	COLORREF line;
};

#define MAX_LOADSTRING 100
using namespace std;
