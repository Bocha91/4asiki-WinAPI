#include <windows.h>
#include "multimon.h"    

#define MONITOR_CENTER   0x0001        // center rect to monitor 
#define MONITOR_CLIP     0x0000        // clip rect to monitor 
#define MONITOR_WORKAREA 0x0002        // use monitor work area 
#define MONITOR_AREA     0x0000        // use monitor entire area 

// 
//  ClipOrCenterRectToMonitor 
// 
//  The most common problem apps have when running on a 
//  multimonitor system is that they "clip" or "pin" windows 
//  based on the SM_CXSCREEN and SM_CYSCREEN system metrics. 
//  Because of app compatibility reasons these system metrics 
//  return the size of the primary monitor. 
// 
//  This shows how you use the multi-monitor functions 
//  to do the same thing. 
// 
// Наиболее распространенные проблемные приложения при работе на
// мультимониторная система состоит в том, что они «зажимают» или «закрепляют» окна
// на основе системных показателей SM_CXSCREEN и SM_CYSCREEN.
// Из-за совместимости приложений эти показатели системы
// вернуть размер основного монитора.
//
// Это показывает, как вы используете функции нескольких мониторов
// сделать то же самое.

void ClipOrCenterRectToMonitor(LPRECT prc, UINT flags)
{
    HMONITOR hMonitor;
    MONITORINFO mi;
    RECT        rc;
    int         w = prc->right - prc->left;
    int         h = prc->bottom - prc->top;

    // 
    // get the nearest monitor to the passed rect. 
    // получаем ближайший монитор к переданному прямоугольнику.
    // 
    hMonitor = MonitorFromRect(prc, MONITOR_DEFAULTTONEAREST);

    POINT      pt;
    pt.x = prc->right;
    pt.y = prc->bottom;

    // 
    // получаем ближайший монитор к переданной точке.
    // 
    hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);

    // 
    // get the work area or entire monitor rect. 
    // получить рабочую область или весь прямоугольник монитора.
    // 
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hMonitor, &mi);

    if (flags & MONITOR_WORKAREA)
        rc = mi.rcWork;
    else
        rc = mi.rcMonitor;

    // 
    // center or clip the passed rect to the monitor rect 
    // центрируем или обрезаем переданный прямоугольник на прямоугольник монитора
    // 
    if (flags & MONITOR_CENTER)
    {
        prc->left = rc.left + (rc.right - rc.left - w) / 2;
        prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
        prc->right = prc->left + w;
        prc->bottom = prc->top + h;
    }
    else
    {
        prc->left = max(rc.left, min(rc.right - w, prc->left));
        prc->top = max(rc.top, min(rc.bottom - h, prc->top));
        prc->right = prc->left + w;
        prc->bottom = prc->top + h;
    }
}

void ClipOrCenterWindowToMonitor(HWND hwnd, UINT flags)
{
    RECT rc;
    rc.left   = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rc.right  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    rc.top    = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    
    // мы хотим позицию в правом верхнем углу виртуального экрана

    //GetWindowRect(hwnd, &rc);
    ClipOrCenterRectToMonitor(&rc, flags);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}