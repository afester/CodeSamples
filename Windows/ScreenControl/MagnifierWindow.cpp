


//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0501    
//#endif

#include "pch.h"
#include <windows.h>
#include <wincodec.h>
#include <magnification.h>

#include "LoggingConsole.h"
#include "MagnifierWindow.h"

// For simplicity, the sample uses a constant magnification factor.
#define MAGFACTOR  2.0f
#define RESTOREDWINDOWSTYLES WS_SIZEBOX | WS_SYSMENU | WS_CLIPCHILDREN | WS_CAPTION | WS_MAXIMIZEBOX

// Global variables and strings.
HINSTANCE           hInst;
const TCHAR         WindowClassName[] = TEXT("MagnifierWindow");
const TCHAR         WindowTitle[] = TEXT("Screen Magnifier Sample");
const UINT          timerInterval = 16; // close to the refresh rate @60hz
HWND                hwndMag;
HWND                hwndHost;
RECT                magWindowRect;
RECT                hostWindowRect;
UINT_PTR            timerId;
float               magFactor;
BOOL                isFullScreen = FALSE;
// LoggingConsole*  console;

// Forward declarations.
ATOM                RegisterHostWindowClass(HINSTANCE hInstance);
// BOOL                SetupMagnifier(HINSTANCE hinst);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK       UpdateMagWindow(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void                GoFullScreen();
void                GoPartialScreen();


//
// FUNCTION: HostWndProc()
//
// PURPOSE: Window procedure for the window that hosts the magnifier control.
//
LRESULT CALLBACK HostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            if (isFullScreen)
            {
                //GoPartialScreen();
            }
        }
        break;

    case WM_SYSCOMMAND:
        if (GET_SC_WPARAM(wParam) == SC_MAXIMIZE)
        {
            //GoFullScreen();
        }
        else
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_DESTROY:
        // PostQuitMessage(0);
        break;

    case WM_SIZE:
        if (hwndMag != NULL) {
            Logger->log(L"RESIZE!");

            // get client rect of host window
            GetClientRect(hWnd, &magWindowRect);

            // Resize the mag window to fill entire parent client area
            SetWindowPos(hwndMag, NULL,
                magWindowRect.left, magWindowRect.top, magWindowRect.right, magWindowRect.bottom, 0);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
//  FUNCTION: RegisterHostWindowClass()
//
//  PURPOSE: Registers the window class for the window that contains the magnification control.
//
ATOM RegisterHostWindowClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = HostWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(1 + COLOR_BTNFACE);
    wcex.lpszClassName = WindowClassName;

    return RegisterClassEx(&wcex);
}


BOOL SetupMagnifier(HINSTANCE hinst, HWND screenCtlDlgWnd) { // }, LoggingConsole& console) {
    CString str;

//     ::console = &console;
    Logger->log(L"SETUP MAGNIFIER!");

    if (FALSE == MagInitialize())
    {
        return 0;
    }



    // Set bounds of host window according to screen size.
    hostWindowRect.top = 0;
    hostWindowRect.bottom = GetSystemMetrics(SM_CYSCREEN); //  / 2;  // top half of screen
    hostWindowRect.left = 0;
    hostWindowRect.right = GetSystemMetrics(SM_CXSCREEN);

    // Create the host window.
    RegisterHostWindowClass(hinst);
    hwndHost = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED,
        WindowClassName, WindowTitle,
        RESTOREDWINDOWSTYLES,
        0, 0, hostWindowRect.right, hostWindowRect.bottom, NULL, NULL, hInst, NULL);
    if (!hwndHost)
    {
        return FALSE;
    }
    str.Format(L"Host Window Handle: 0x%x", hwndHost);
    Logger->log(str);

    // Make the window opaque (hint: make slightly transparent for debugging purposes!)
    SetLayeredWindowAttributes(hwndHost, 0, 255, LWA_ALPHA);

    // Create a magnifier window that fills the client area.
    GetClientRect(hwndHost, &magWindowRect);
    hwndMag = CreateWindow(WC_MAGNIFIER, TEXT("MagnifierWindow"),
        WS_CHILD | MS_SHOWMAGNIFIEDCURSOR | WS_VISIBLE,
        magWindowRect.left, magWindowRect.top, magWindowRect.right, magWindowRect.bottom,
        //20, 20,   // Location of the mag window inside its host window
        //1000, 500,  // Size of the Mag window
        hwndHost, NULL, hInst, NULL);
    if (!hwndMag)
    {
        str.Format(L"Magnifier Control Creation ERROR: 0x%x", GetLastError());
        Logger->log(str);
        return FALSE;
    }
    str.Format(L"Magnifier Control Handle: 0x%x", hwndHost);
    Logger->log(str);

    // exclude the screen control dialog from magnification
    MagSetWindowFilterList(hwndMag, MW_FILTERMODE_EXCLUDE, 1, &screenCtlDlgWnd);

    SetMagFactor(1.0);

    //if (ret)
    //{
    //    MAGCOLOREFFECT magEffectInvert =
    //    { { // MagEffectInvert
    //        { -1.0f,  0.0f,  0.0f,  0.0f,  0.0f },
    //        {  0.0f, -1.0f,  0.0f,  0.0f,  0.0f },
    //        {  0.0f,  0.0f, -1.0f,  0.0f,  0.0f },
    //        {  0.0f,  0.0f,  0.0f,  1.0f,  0.0f },
    //        {  1.0f,  1.0f,  1.0f,  0.0f,  1.0f }
    //    } };
    //    MAGCOLOREFFECT magEffectGrayscale =
    //    { { // MagEffectGrayscale
    //        {  0.3f,  0.3f,  0.3f,  0.0f,  0.0f },
    //        {  0.6f,  0.6f,  0.6f,  0.0f,  0.0f },
    //        {  0.1f,  0.1f,  0.1f,  0.0f,  0.0f },
    //        {  0.0f,  0.0f,  0.0f,  1.0f,  0.0f },
    //        {  0.0f,  0.0f,  0.0f,  0.0f,  1.0f }
    //    } };
    //    ret = MagSetColorEffect(hwndMag, &magEffectGrayscale);
    //}

    ShowWindow(hwndHost, SW_SHOW); //  nCmdShow);
    UpdateWindow(hwndHost);


    // Create a timer to update the control.
    //timerId = SetTimer(hwndHost, 0, timerInterval, UpdateMagWindow);

    // SetWindowPos() so that the client area(!!) of the host window occupies the complete desktop
    GoFullScreen();

    return TRUE;
}

void SetMagFactor(float factor) {
    magFactor = factor;

    // Set the magnification factor.
    MAGTRANSFORM matrix;
    memset(&matrix, 0, sizeof(matrix));
    matrix.v[0][0] = factor;
    matrix.v[1][1] = factor;
    matrix.v[2][2] = 1.0f;

    BOOL ret = MagSetWindowTransform(hwndMag, &matrix);

    POINT center;
    //center.x = 2560 / 2;
    //center.y = 1440 / 2;
    center.x = 1920 / 2;
    center.y = 1080 / 2;

    SetMagWindowCenter(center);
}

void StopMagnifier() {
    // Shut down.
    //KillTimer(NULL, timerId);
 //   DestroyWindow(hwndMag);
    DestroyWindow(hwndHost);
    MagUninitialize();
}


//
// FUNCTION: UpdateMagWindow()
//
// PURPOSE: Sets the source rectangle and updates the window. Called by a timer.
//
//void CALLBACK UpdateMagWindow(HWND /*hwnd*/, UINT /*uMsg*/, UINT_PTR /*idEvent*/, DWORD /*dwTime*/)
//{
//    POINT mousePoint;
//    GetCursorPos(&mousePoint);
//}

void SetMagWindowCenter(const POINT& center) {
    CString str;

    // widht/height of magnified desktop area
    // right-left => width of mag window; / magFactor => width of desktop area. Same for bottom/top.
    str.Format(L"MAG WINDOW size: %d/%d - %d/%d", magWindowRect.left, magWindowRect.top,
        magWindowRect.right, magWindowRect.bottom);
    Logger->log(str);

    int width = (int)((magWindowRect.right - magWindowRect.left) / magFactor);
    int height = (int)((magWindowRect.bottom - magWindowRect.top) / magFactor);
    str.Format(L"DESKTOP AREA size: %dx%d", width, height);
    Logger->log(str);

    // calculate the rectangle to be magnified, in desktop coordinates.
    RECT sourceRect;
    sourceRect.left = center.x - width / 2;
    sourceRect.top = center.y - height / 2;

    // Don't scroll outside desktop area.
    if (sourceRect.left < 0) {
        sourceRect.left = 0;
    }
    if (sourceRect.left > GetSystemMetrics(SM_CXSCREEN) - width) {
        sourceRect.left = GetSystemMetrics(SM_CXSCREEN) - width;
    }
    sourceRect.right = sourceRect.left + width;

    if (sourceRect.top < 0) {
        sourceRect.top = 0;
    }
    if (sourceRect.top > GetSystemMetrics(SM_CYSCREEN) - height) {
        sourceRect.top = GetSystemMetrics(SM_CYSCREEN) - height;
    }
    sourceRect.bottom = sourceRect.top + height;

    str.Format(L"SOURCE RECT: %d/%d - %d/%d", sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom);
    Logger->log(str);

    //RECT sourceRect;
    //sourceRect.left = 560;
    //sourceRect.right= 2000;
    //sourceRect.top = 300;
    //sourceRect.bottom = 1140;

    // Reclaim topmost status, to prevent unmagnified menus from remaining in view. 
    //SetWindowPos(hwndHost, HWND_TOPMOST, 0, 0, 0, 0,
    //    SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

        // Set the source rectangle for the magnifier control.
    MagSetWindowSource(hwndMag, sourceRect);

//    SetWindowPos(hwndMag, HWND_TOPMOST, 300, 300, 0, 0, 0);
        // SWP_NOACTIVATE | SWP_NOSIZE);


    // Force redraw (required??)
    InvalidateRect(hwndMag, NULL, TRUE);
}


//
// FUNCTION: GoFullScreen()
//
// PURPOSE: Makes the host window full-screen by placing non-client elements outside the display.
//
void GoFullScreen()
{
    isFullScreen = TRUE;
    // The window must be styled as layered for proper rendering. 
    // It is styled as transparent so that it does not capture mouse clicks.
    SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT);
    // Give the window a system menu so it can be closed on the taskbar.
    SetWindowLong(hwndHost, GWL_STYLE, WS_CAPTION | WS_SYSMENU);

    // Calculate the span of the display area.
    HDC hDC = GetDC(NULL);
    int xSpan = GetSystemMetrics(SM_CXSCREEN);
    int ySpan = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(NULL, hDC);

    // Calculate the size of system elements.
    int xBorder = GetSystemMetrics(SM_CXFRAME);
    int yCaption = GetSystemMetrics(SM_CYCAPTION);
    int yBorder = GetSystemMetrics(SM_CYFRAME);

    CString str;
    str.Format(L"xBORDER: %d px, yBORDER: %d px, yCaption: %d px", xBorder, yBorder, yCaption);
    Logger->log(str);

    // Calculate the window origin and span for full-screen mode.
    int xOrigin = 0;    //  -xBorder;           // 0 and -11 resulted in a perfect match of the mag window with the desktop for zoom factor 1 (??)
    int yOrigin = -11;  //  -yBorder - yCaption;
    xSpan += 2 * xBorder;
    ySpan += 2 * yBorder + yCaption;

    SetWindowPos(hwndHost, HWND_TOPMOST, xOrigin, yOrigin, xSpan, ySpan,
        SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}

#if 0

//
// FUNCTION: GoPartialScreen()
//
// PURPOSE: Makes the host window resizable and focusable.
//
void GoPartialScreen()
{
    isFullScreen = FALSE;

    SetWindowLong(hwndHost, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED);
    SetWindowLong(hwndHost, GWL_STYLE, RESTOREDWINDOWSTYLES);
    SetWindowPos(hwndHost, HWND_TOPMOST,
        hostWindowRect.left, hostWindowRect.top, hostWindowRect.right, hostWindowRect.bottom,
        SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOACTIVATE);
}

#endif
