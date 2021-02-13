#include "scrolling.h"

void VertScroll(TView* view, WPARAM wParam)
{
    int vertScrollInc = 0;

    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
        if (view->vertScrollPos != 0)
            vertScrollInc = -1 * view->sizeVertScroll;
        break;
    case SB_LINEDOWN:
        if (view->vertScrollPos != view->lastVertPos)
            vertScrollInc = view->sizeVertScroll;
        break;
    case SB_PAGEUP:
        vertScrollInc = min(-1, - 1 * view->countLines);
        break;
    case SB_PAGEDOWN:
        vertScrollInc = max(1, view->countLines);
        break;
    case SB_THUMBTRACK:
        view->physVertPos = HIWORD(wParam);
        if (view->lastVertPos > USHRT_MAX)
            view->physVertPos = (int)((double)view->physVertPos / (double)USHRT_MAX * (double)view->lastVertPos);
        vertScrollInc = view->physVertPos - view->vertScrollPos;
        break;
    default:
        vertScrollInc = 0;
        break;
    }

    if (vertScrollInc != 0)
    {
        vertScrollInc = max(-1 * (int)view->vertScrollPos, min(vertScrollInc, (int)view->lastVertPos - view->vertScrollPos));
        view->vertScrollPos += vertScrollInc;

        ScrollWindow(view->hwnd, 0, -1 * view->heightChar * vertScrollInc, NULL, NULL);

        if (view->lastVertPos > USHRT_MAX)
            view->physVertPos = (int)((double)view->vertScrollPos / (double)view->lastVertPos * (double)USHRT_MAX);
        else
            view->physVertPos = view->vertScrollPos;

        SetScrollPos(view->hwnd, SB_VERT, view->physVertPos, TRUE);
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
            horzScrollInc = -1 * view->sizeHorzScroll;
        break;
    case SB_LINEDOWN:
        if (view->horzScrollPos != view->lastHorzPos)
            horzScrollInc = view->sizeHorzScroll;
        break;
    case SB_PAGEUP:
        horzScrollInc = min(-1, -1 * view->countChars);
        break;
    case SB_PAGEDOWN:
        horzScrollInc = max(1, view->countChars);
        break;
    case SB_THUMBTRACK:
        view->physHorzPos = HIWORD(wParam);
        if (view->lastHorzPos > USHRT_MAX)
            view->physHorzPos = (int)((double)view->physHorzPos / (double)USHRT_MAX * (double)view->lastHorzPos);
        horzScrollInc = view->physHorzPos - view->horzScrollPos;
        break;
    default:
        horzScrollInc = 0;
        break;
    }

    if (horzScrollInc != 0)
    {
        horzScrollInc = max(-1 * (int)view->horzScrollPos, min(horzScrollInc, (int)view->lastHorzPos - view->horzScrollPos));
        view->horzScrollPos += horzScrollInc;

        ScrollWindow(view->hwnd, -1 * view->widthChar * horzScrollInc, 0, NULL, NULL);

        if (view->lastHorzPos > USHRT_MAX)
            view->physHorzPos = (int)((double)view->horzScrollPos / (double)view->lastHorzPos * (double)USHRT_MAX);
        else
            view->physHorzPos = view->horzScrollPos;

        SetScrollPos(view->hwnd, SB_HORZ, view->physHorzPos, TRUE);
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
    case VK_PRIOR:
        SendMessage(view->hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
        break;
    case VK_NEXT:
        SendMessage(view->hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
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
