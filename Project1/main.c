#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include "TModel.h"
#include "TView.h"
#include "processing.h"
#include "changingSize.h"
#include "scrolling.h"
#include "menu.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "menu";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument         /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CREATESTRUCT* cs;
    static TModel* model;
    static TView* view;
    HMENU hMenu;

    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:

            cs = (CREATESTRUCT*)lParam;

            model = InitModel((char*)cs->lpCreateParams);
            view = InitView(hwnd);

            FillOffset(model);
            FillMetrics(view);
            MaxLineLength(model, view);

            break;

        case WM_PAINT:

            if (model && view)
                PrintText(model, view);

            break;

        case WM_SIZE:

            ChangeVertSize(view, model, lParam);
            ChangeHorzSize(view, model, lParam);
            InvalidateRect(view->hwnd, NULL, TRUE);

            break;

        case WM_VSCROLL:

            VertScroll(view, wParam);

            break;

        case WM_HSCROLL:

            HorzScroll(view, wParam);

            break;

        case WM_KEYDOWN:

            ArrowsDown(view, wParam);

            break;

        case WM_CHAR:

            CharsDown(view, wParam);

            break;

        case WM_COMMAND:

            hMenu = GetMenu(view->hwnd);

            switch(LOWORD(wParam))
            {
            case IDM_OPEN:
                EnableMenuItem(hMenu, IDM_CLOSE, MF_ENABLED);
                EnableMenuItem(hMenu, IDM_VIEW, MF_ENABLED | MF_BYPOSITION);
                break;
            case IDM_CLOSE:
                EnableMenuItem(hMenu, IDM_CLOSE, MF_GRAYED);
                EnableMenuItem(hMenu, IDM_VIEW, MF_GRAYED | MF_BYPOSITION);
                break;
            case IDM_EXIT:
                SendMessage(view->hwnd, WM_CLOSE, 0, 0L);
                break;

            case IDM_SIMPLE:
                CheckMenuItem(hMenu, IDM_SIMPLE, MF_CHECKED);
                CheckMenuItem(hMenu, IDM_LAYOUT, MF_UNCHECKED);
                break;
            case IDM_LAYOUT:
                CheckMenuItem(hMenu, IDM_LAYOUT, MF_CHECKED);
                CheckMenuItem(hMenu, IDM_SIMPLE, MF_UNCHECKED);
                break;
            }

            break;

        case WM_DESTROY:

            DeleteModel(model);
            DeleteView(view);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */

            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
