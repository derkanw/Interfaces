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
        if (view->vertScrollPos != view->lastVertPos)
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

    if (vertScrollInc != 0)
    {
        view->vertScrollPos += vertScrollInc;
        view->vertScrollPos = max(0, min(view->vertScrollPos, (int)view->lastVertPos));
        SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);
        InvalidateRect(view->hwnd, NULL, TRUE);
        UpdateWindow(view->hwnd);
    }
}

void HorzScroll(TView* view, WPARAM wParam)
{
    int horzScrollInc = 0;

    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        if (view->horzScrollPos != 0)
            horzScrollInc = -view->sizeHorzScroll;
        break;
    case SB_LINEDOWN:
        if (view->horzScrollPos != view->lastHorzPos)
            horzScrollInc = view->sizeHorzScroll;
        break;
    case SB_PAGEUP:
        horzScrollInc = -view->countChars;
        break;
    case SB_PAGEDOWN:
        horzScrollInc = view->countChars;
        break;
    case SB_THUMBTRACK:
        horzScrollInc = HIWORD(wParam) - view->horzScrollPos;
        break;
    default:
        horzScrollInc = 0;
        break;
    }

    if (horzScrollInc != 0)
    {
        view->horzScrollPos += horzScrollInc;
        view->horzScrollPos = max(0, min(view->horzScrollPos, (int)view->lastHorzPos));
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
