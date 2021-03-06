// Graph.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Graph.h"

Vertex masVer[100]; // массив вершин
Rib masRib[1024]; // массив ребер

// Глобальные переменные:
HINSTANCE hInst;    
HWND hWnd;// текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
TCHAR g_szGrafClass[] = TEXT("GrafClass");
int num = 0;
int cnt = 0;
bool flag = false;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    GetRes(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AddRib(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK GraphProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(brBackground + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GRAPH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPH));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   const TCHAR* str[NUM_TBBUTTONS] = { TEXT("Добавить вершину"), TEXT("Добавить ребро"), TEXT("Найти пути") };
   for (int i = 0; i < NUM_TBBUTTONS; i++) {
	   tbButtons[i].iString = (INT_PTR)str[i];
   }
   HWND hWndToolBar = CreateToolbarEx(hWnd,
	   WS_CHILD | TBSTYLE_LIST |  TBSTYLE_TOOLTIPS | WS_CLIPCHILDREN | TBSTYLE_WRAPABLE|
	   WS_BORDER| WS_VISIBLE,
	   ID_TOOLBAR, //Идентификатор ТулБара
	   NUM_TBBITMAPS,  //Количество картинок
	   (HINSTANCE)hInst,  //Module instance with the executable file that contains the bitmap resource.
	   IDR_TOOLBAR1,   //Ресурсы
	   (LPCTBBUTTON)&tbButtons,  //Кнопочки
	   NUM_TBBUTTONS,      //количество кнопок
	   BMP_CX, BMP_CY,                  // width & height of buttons
	   0, 0,                  // width & height of bitmaps
	   sizeof(TBBUTTON));

   if (hWndToolBar == NULL)
   {
	   MessageBox(NULL, TEXT("Toolbar not created!"), NULL, MB_OK);
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	case WM_CREATE: 
		{
			if (!RegClass(GraphProc, g_szGrafClass, COLOR_WINDOW)) {
				MessageBox(NULL, TEXT("Не могу зарегистрировать класс вершин графа."), TEXT("Ошибка"), MB_OK);
				return false;
			}
			

			//ShowWindow(hTemp, SW_SHOW);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
			for (int i = 0; i < num; i++)
				if (GetFocus() == masVer[i].hWnd)
					MessageBox(NULL, TEXT("Не могу зарегистрировать класс вершин графа."), TEXT("Ошибка"), MB_OK);
            switch (wmId)
            {
			case ID_TOP: 
			{
				Vertex ver;
				ver.num = num;
				ver.hWnd = CreateWindowEx(
					0, g_szGrafClass,   // predefined class 
					TEXT(""),         // no window title 
					WS_CHILD | WS_VISIBLE,
					0, 50, 50, 50,   // set size in WM_SIZE message 
					hWnd,         // parent window 
					NULL,   // edit control ID 
					(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
					NULL);
				masVer[num] = ver;
				num++;
			}
				break;
			case ID_RIB:
			{
				/*
				masRib[0].ver.x = 0;
				masRib[0].ver.y = 1;
				masRib[0].line = RGB(0, 0, 255);
				cnt++;
				InvalidateRect(hWnd, NULL, TRUE);
				*/
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDEDGE), hWnd, AddRib);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			case ID_GET:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_GETBOX), hWnd, GetRes);
				break;
			case IDM_ABOUT: 
			/*{
				for (int i = 0; i < num; i++) 
				{
					TCHAR buf[10] = TEXT("");
					_stprintf_s(buf, TEXT("%d"), masVer[i].num);
					MessageBox(NULL, buf, TEXT("Ошибка"), MB_OK);
				}
				
			}*/
				
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		PAINTSTRUCT pt;

		HDC hdc = BeginPaint(hWnd, &pt);
		for (int i = 0; i < cnt; i++) {
			HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 2, masRib[i].line));
			POINT p1, p2;
			RECT Rect;
			p1.x = p1.y = p2.x = p2.y = 0;

			for (int j = 0; j < num; j++) {
				if (masRib[i].ver.x == masVer[j].num) {
					GetWindowRect(masVer[j].hWnd, &Rect);
					MapWindowPoints(HWND_DESKTOP, GetParent(masVer[j].hWnd), (LPPOINT)&Rect, 2);

					p1.x = Rect.left + 25;
					p1.y = Rect.top + 25;

				}
				if (masRib[i].ver.y == masVer[j].num) {
					GetWindowRect(masVer[j].hWnd, &Rect);
					MapWindowPoints(HWND_DESKTOP, GetParent(masVer[j].hWnd), (LPPOINT)&Rect, 2);
					p2.x = Rect.left + 25;
					p2.y = Rect.top + 25;
				}
			}
			MoveToEx(hdc, p2.x, p2.y, NULL);
			LineTo(hdc, p1.x, p1.y);
			DeleteObject(SelectObject(hdc, hPen));
		}

		EndPaint(hWnd, &pt);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK GraphProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) {
	case WM_PAINT: 
	{
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rc;
		HPEN hPen;
		HFONT hFont;
		TCHAR buf[10] = TEXT("");

		static LOGFONT lf;
		lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
		lf.lfItalic = FALSE;
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = 25;
		lf.lfCharSet = DEFAULT_CHARSET;

		rc.left = 0;
		rc.right = 50;
		rc.top = 0;
		rc.bottom = 50;

		for (int i = 0; i < num; i++){
			if (hWnd == masVer[i].hWnd)
				_stprintf_s(buf, TEXT("%d"), masVer[i].num + 1);
			}
				
		hDC = BeginPaint(hWnd, &ps);

		hPen = (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 2, RGB(0, 0, 0)));
		hFont = (HFONT)SelectObject(hDC, CreateFontIndirect(&lf));

		Ellipse(hDC, 0, 0, 50 - 3, 50 - 3);
		DrawText(hDC, buf, _tcslen(buf), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		DeleteObject(SelectObject(hDC, hPen));
		DeleteObject(SelectObject(hDC, hFont));
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_LBUTTONDOWN:
	{
		SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		InvalidateRect(GetParent(hWnd), NULL, TRUE);

	}
	break;
	case WM_MOUSEMOVE:
	{
		
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK GetRes(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND Сombo1, Combo2, Combo3,  hBut1, hBut2;
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
	case WM_INITDIALOG: {
		Сombo1 = GetDlgItem(hDlg, IDC_COMBO1);
		Combo2 = GetDlgItem(hDlg, IDC_COMBO2);
		Combo3 = GetDlgItem(hDlg, IDC_COMBO3);
		hBut1 = GetDlgItem(hDlg, IDC_BUTTON1);
		hBut2 = GetDlgItem(hDlg, IDC_BUTTON2);
		EnableWindow(hBut1, TRUE);
		EnableWindow(hBut2, TRUE);
		return (INT_PTR)TRUE;
	}
        

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK AddRib(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND Combo1, Combo2, hBut1, hBut2;
	TCHAR buf[10] = TEXT("");
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		Combo1 = GetDlgItem(hDlg, IDC_COMBO4);
		Combo2 = GetDlgItem(hDlg, IDC_COMBO5);
		hBut1 = GetDlgItem(hDlg, IDC_BUTTON3);
		hBut2 = GetDlgItem(hDlg, IDC_BUTTON4);
		EnableWindow(hBut1, TRUE);
		EnableWindow(hBut2, TRUE);

		for (int i = 0; i< num; i++) {
			_stprintf_s(buf, TEXT("%d"), masVer[i].num + 1);
			ComboBox_AddString(Combo1, buf);
			ComboBox_AddString(Combo2, buf);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDC_BUTTON3:
			{
				int id1 = SendMessage(Combo1, CB_GETCURSEL, 0, 0);
				int id2 = SendMessage(Combo2, CB_GETCURSEL, 0, 0);

				if (id1 == id2) {
					MessageBox(NULL, TEXT("Невозможно создать связь!"), TEXT("Ошибка"), MB_OK);
					return (INT_PTR)TRUE;
				}
				else {
					bool flag = true;
					for (int i = 0; i < cnt; i++) {
						if (masRib[i].ver.x == id1 && masRib[i].ver.y == id2
							|| masRib[i].ver.x == id2 && masRib[i].ver.y == id1) {
							flag = false;
							MessageBox(NULL, TEXT("Связь уже существует!"), TEXT("Ошибка"), MB_OK);
							return (INT_PTR)TRUE;
						}
					}
					if (flag) {
						masRib[cnt].ver.x = id1;
						masRib[cnt].ver.y = id2;
						masRib[cnt].line = RGB(0, 0, 0);
						cnt++;
					}
				}
			}
			return (INT_PTR)TRUE;
			case IDC_BUTTON4: 
			{
				int id1 = SendMessage(Combo1, CB_GETCURSEL, 0, 0);
				int id2 = SendMessage(Combo2, CB_GETCURSEL, 0, 0);

				if (id1 == id2) {
					MessageBox(NULL, TEXT("Невозможно удалить связь!"), TEXT("Ошибка"), MB_OK);
					return (INT_PTR)TRUE;
				}
				else {
					bool flag = true;
					for (int i = 0; i < cnt; i++) {
						if (masRib[i].ver.x == id1 && masRib[i].ver.y == id2
							|| masRib[i].ver.x == id2 && masRib[i].ver.y == id1) {
							flag = false;
							masRib[i].ver.x = masRib[cnt - 1].ver.x;
							masRib[i].ver.y = masRib[cnt - 1].ver.y;
							cnt--;
							return (INT_PTR)TRUE;
						}
					}
					if (flag) {
						MessageBox(NULL, TEXT("Связи не существует!"), TEXT("Ошибка"), MB_OK);
					}
				}
			}
				return (INT_PTR)TRUE;
			case IDOK:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

