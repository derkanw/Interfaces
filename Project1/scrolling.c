#include "TModel.h"
#include "TView.h"

void VertScroll(TView* view, WPARAM wParam)
{
    int vertScrollInc = 0;

    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        if (view->vertScrollPos != 0)
            vertScrollInc = -view->sizeVertScroll;
        break;
    case SB_LINEDOWN:
        vertScrollInc = view->sizeVertScroll;
        break;
    case SB_PAGEUP:
        vertScrollInc = min(-1, -view->countLines);
        break;
    case SB_PAGEDOWN:
        vertScrollInc = max(1, view->countLines);
        break;
    case SB_THUMBTRACK:
        vertScrollInc = HIWORD(wParam) - view->vertScrollPos;
        break;
    default:
        vertScrollInc = 0;
        break;
    }

    vertScrollInc = max(-view->vertScrollPos, min(vertScrollInc, view->lastVertPos - view->vertScrollPos));

    if (vertScrollInc != 0)
        {
            //view->vertScrollPos = max(0, min(view->vertScrollPos, view->lastVertPos));
            view->vertScrollPos += vertScrollInc;

            ScrollWindow(view->hwnd, 0, -view->heightChar * vertScrollInc, NULL, NULL);
            SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);
            //InvalidateRect(view->hwnd, NULL, TRUE);
            UpdateWindow(view->hwnd);
        }
}

void HorzScroll(TView* view, WPARAM wParam)
{
    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        if (view->horzScrollPos != 0)
            view->horzScrollPos -= view->sizeHorzScroll;
        break;
    case SB_LINEDOWN:
        view->horzScrollPos += view->sizeHorzScroll;
        break;
    case SB_PAGEUP:
        view->horzScrollPos -= view->countChars;
        break;
    case SB_PAGEDOWN:
        view->horzScrollPos += view->countChars;
        break;
    case SB_THUMBTRACK:
        view->horzScrollPos += HIWORD(wParam) - view->horzScrollPos;
        break;
    default:
        break;
    }
    view->horzScrollPos = max(0, min(view->horzScrollPos, view->lastHorzPos));

    if (view->horzScrollPos != GetScrollPos(view->hwnd, SB_HORZ))
    {
        SetScrollPos(view->hwnd, SB_HORZ, view->horzScrollPos, TRUE);
        InvalidateRect(view->hwnd, NULL, TRUE);
        UpdateWindow(view->hwnd);
    }
}

void ArrowsDown(TView* view, WPARAM wParam)
{
    switch (wParam)
    {
    case VK_UP:
        SendMessage(view->hwnd, WM_VSCROLL, SB_LINEUP, 0L);
        break;
    case VK_DOWN:
        SendMessage(view->hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
        break;
    case VK_RIGHT:
        SendMessage(view->hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
        break;
    case VK_LEFT:
        SendMessage(view->hwnd, WM_HSCROLL, SB_LINEUP, 0L);
        break;
    }
}

void CharsDown(TView* view, WPARAM wParam)
{
    switch (wParam)
    {
    case 'w':
        SendMessage(view->hwnd, WM_VSCROLL, SB_LINEUP, 0L);
        break;
    case 's':
        SendMessage(view->hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
        break;
    case 'd':
        SendMessage(view->hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
        break;
    case 'a':
        SendMessage(view->hwnd, WM_HSCROLL, SB_LINEUP, 0L);
        break;
    }
}