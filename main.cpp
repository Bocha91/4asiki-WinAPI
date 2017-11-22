#include <windows.h> 
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h> 
#include <time.h>
#include "resource.h"
// Global variables

// The main window class name. 
static TCHAR szWindowClass[] = _T("Часики на WinAPI"); 
// The string that appears in the application's title bar. 
//static TCHAR szTitle[] = _T("Часики WinAPI");
static TCHAR szTitle[] = _T("Часики на WinAPI");
//TCHAR iconName[] = _T("IDI_MYICON");

HINSTANCE hInst;
int cxChar, cyChar;
RECT rc;
TEXTMETRIC tm;

// Forward declarations of functions included in this code module: 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{ 
	WNDCLASSEX wcex; 
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style  = CS_HREDRAW | CS_VREDRAW; 
	wcex.lpfnWndProc    = WndProc; 
	wcex.cbClsExtra     = 0; 
	wcex.cbWndExtra     = 0; 
	wcex.hInstance      = hInstance; 
//	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW); 
	wcex.hbrBackground  = (HBRUSH)(COLOR_ACTIVECAPTION);
	//wcex.hbrBackground  = (HBRUSH)(COLOR_BACKGROUND);
	wcex.lpszMenuName   = NULL; 
	wcex.lpszClassName  = szWindowClass; 
//	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); 
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MYICON));
	if (!RegisterClassEx(&wcex))
	{ 
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Часики WinAPI"), NULL);
		return 1; 
	} 
	hInst = hInstance; 
	HWND hWnd = CreateWindowEx(WS_EX_TOPMOST,szWindowClass, szTitle, WS_POPUP /*| WS_THICKFRAME*/, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{ 
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Часики WinAPI"), NULL);
		return 1; 
	} 
	
	// The parameters to ShowWindow explained: 
	// hWnd: the value returned from CreateWindow 
	// nCmdShow: the fourth parameter from WinMain 
	ShowWindow(hWnd, nCmdShow); 
//	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~(WS_CAPTION | WS_BORDER | WS_THICKFRAME));
	UpdateWindow(hWnd);
/*
	UINT nTimerID; // Возвращаемое значение SetTimer
	nTimerID = SetTimer(hWnd,1,700,(TIMERPROC)NULL);      // no timer callback 
	if (nTimerID == 0)
	{
		MessageBox(NULL, _T("Call to SetTimer failed!"), _T("Часики WinAPI"), NULL);
	}
*/
	// Получаем координаты окна Desktop.
	// Это окно занимает всю поверхность экрана,
	// и на нем расположены все остальные окна
	GetWindowRect(GetDesktopWindow(), &rc);

	// Передвигаем окно в правый верхний угол экрана
	MoveWindow(hWnd,
		rc.right - cxChar*8 -10,
		rc.top + 0,
		cxChar*8 +10, cyChar, TRUE);

	// Main message loop:
	MSG msg; 
	while (GetMessage(&msg, NULL, 0, 0)) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 
	return (int) msg.wParam; 
}

HFONT hFont = NULL;
HFONT hFontOld = NULL;
BOOL AddFont = FALSE;
const TCHAR *pFontFile = _T("digital-7_mono.ttf"); // чтобы шрифт работал положи его в папку из которой запускаеш эти часики
const TCHAR *pFontName = _T("digital-7 mono");
// --------------------------------------------------------------------------
int CALLBACK EnumFontsProc
(
	CONST LOGFONT *lplf,     // logical-font data
	CONST TEXTMETRIC *lptm,  // physical-font data
	DWORD dwType,            // font type
	LPARAM lpData            // application-defined data
	)
{
	::OutputDebugString(lplf->lfFaceName);
	return 1;
}
// 
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM) 
// 
//  PURPOSE:  Processes messages for the main window. 
// 
//  WM_PAINT    - Paint the main window 
//  WM_DESTROY  - post a quit message and return 
// 
// 
TCHAR greeting[] = _T("  Hello, World! ");
DWORD color;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
	PAINTSTRUCT ps; 
	HDC hdc; 
	switch (message) 
	{ 
		case WM_CREATE:
		{
			// Создаем таймер, посылающий сообщения функции окна примерно раз в секунду
			SetTimer(hWnd, 1, 1000, NULL);


//*********
			if (::AddFontResource(pFontFile) != 0)
			{
				AddFont = TRUE;
				//::PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);  // сообщить о шрифте

				hFont = ::CreateFont
					(
						120,                     // height of font 
						0,                      // average character width
						0,                      // angle of escapement
						0,                      // base-line orientation angle
						FW_NORMAL,             // font weight
						FALSE,                  // italic attribute option
						FALSE,                  // underline attribute option
						FALSE,                  // strikeout attribute option
						DEFAULT_CHARSET,        // character set identifier
						OUT_DEFAULT_PRECIS,//OUT_TT_PRECIS,                      // output precision
						CLIP_DEFAULT_PRECIS,                      // clipping precision
						DEFAULT_QUALITY,//PROOF_QUALITY,//CLEARTYPE_QUALITY, // output quality
						DEFAULT_PITCH | FF_SWISS,//FF_DECORATIVE,//FF_SWISS,         // pitch and family
						pFontName               // typeface name
						);//_T("Arial"));


				HDC hdc = ::GetDC(hWnd);
				::EnumFonts
					(
						hdc,               // handle to DC
						NULL,              //LPCTSTR lpFaceName,       // font typeface name
						EnumFontsProc,     //FONTENUMPROC lpFontFunc,  // callback function
						0                  //LPARAM lParam             // application-supplied data
						);

				if (hFont) { SelectObject(hdc, hFont); }
				else 	SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
				// Заполняем структуру информацией о метрике шрифта, выбранного в контекст отображения
				GetTextMetrics(hdc, &tm);

				// Запоминаем значение ширины для самого широкого символа
				cxChar = tm.tmMaxCharWidth;

				// Запоминаем значение высоты букв с учетом межстрочного интервала
				cyChar = tm.tmHeight + tm.tmExternalLeading;
				ReleaseDC(hWnd, hdc);
			}
			else {

				//*********
				hdc = GetDC(hWnd);

				// Выбираем шрифт с фиксированной шириной букв
				SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

				// Заполняем структуру информацией о метрике шрифта, выбранного в контекст отображения
				GetTextMetrics(hdc, &tm);

				// Запоминаем значение ширины для самого широкого символа
				cxChar = tm.tmMaxCharWidth;

				// Запоминаем значение высоты букв с учетом межстрочного интервала
				cyChar = tm.tmHeight + tm.tmExternalLeading;

				ReleaseDC(hWnd, hdc);
			}
			break;
		}

		case WM_TIMER:
			//greeting[0] += 1;
			//MessageBeep(-1); 
			InvalidateRect(hWnd, NULL, FALSE);
			break;
			// Для обеспечения возможности перемещения окна, не имеющего заголовка, встраиваем свой обработчик сообщения WM_NCHITTEST
		case WM_NCHITTEST:
		{
			LRESULT lRetVal;

			// Вызываем функцию DefWindowProc и проверяем возвращаемое ей значение
			lRetVal = DefWindowProc(hWnd, message, wParam, lParam);

			// Если курсор мыши находится на одном из
			// элементов толстой рамки, предназначенной
			// для изменения размера окна, возвращаем
			// неизмененное значение, полученное от
			// функции DefWindowProc
			if (lRetVal == HTLEFT ||
				lRetVal == HTBORDER ||
				lRetVal == HTRIGHT ||
				lRetVal == HTTOP ||
				lRetVal == HTBOTTOM ||
				lRetVal == HTBOTTOMRIGHT ||
				lRetVal == HTTOPRIGHT ||
				lRetVal == HTTOPLEFT ||
				lRetVal == HTBOTTOMLEFT
				)
			{
				return lRetVal;
			}

			// В противном случае возвращаем значение HTCAPTION, которое соответствует заголовку окна.
			else
			{
				return HTCAPTION;
			}
		}
		//case WM_SIZE:
		case WM_ACTIVATE:
			if (wParam) {
				SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | (WS_THICKFRAME | WS_CAPTION | WS_SYSMENU));
			}else {
				SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~(WS_THICKFRAME | WS_CAPTION | WS_SYSMENU));
			}
			return 0;


		case WM_PAINT:
#if 0
			hdc = BeginPaint(hWnd, &ps); 
			TextOut(hdc, 5, 5, greeting, _tcslen(greeting)); 
			EndPaint(hWnd, &ps); 
#else
			//LPWSTR   szBuf[80];
			int    nBufSize;
			time_t t;
			struct tm *ltime;
			RECT    rc;
			DWORD bg;
			hdc = BeginPaint(hWnd, &ps);

			// Определяем время и его отдельные компоненты
			time(&t);
			ltime = localtime(&t);

			// Подготавливаем буфер, заполняя его
			// строкой с текущим временем
			nBufSize = wsprintf(greeting, _T("%02d:%02d:%02d"),
				ltime->tm_hour,
				ltime->tm_min,
				ltime->tm_sec);

			// Выбираем шрифт с фиксированной шириной букв
			if (hFont) { SelectObject(hdc, hFont); }
			else { SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT)); }

			// Получаем координаты и размер окна
			GetClientRect(hWnd, &rc);

			bg = GetSysColor(COLOR_ACTIVECAPTION-1);
			SetBkColor(hdc, bg);
			SetTextColor(hdc, 0xFFFFFF-bg);
			
			// Выводим время в центре окна
			DrawText(hdc, greeting, nBufSize, &rc,
				DT_CENTER | DT_VCENTER |
				DT_NOCLIP | DT_SINGLELINE);

			EndPaint(hWnd, &ps);
#endif
			break; 
		case WM_DESTROY:
			KillTimer(hWnd, 1);
			if (AddFont == TRUE)
			{
				if (::RemoveFontResource(pFontFile) != 0){}
			}
			if (hFont) { ::DeleteObject(hFont); }

			PostQuitMessage(0);
			break; 
		default: 
			return DefWindowProc(hWnd, message, wParam, lParam); 
			break; 
	} 
	return 0; 
} 