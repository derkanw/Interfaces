#include "processing.h"
#include "supporting.h"
#include "../Open/main.h"

int PreparePrint(TModel* model, TView* view, char* filename)
{
    if (FillModel(&model->str, &model->length, filename))
        return 1;

    FillOffset(model);
    MaxLineLength(model, view);

    SendMessage(view->hwnd, WM_SIZE, 0, 0);
    LayoutMode(model, view);

    return 0;
}

int OpenNewFile(TModel* model, TView* view)
{
    if (GetOpenFileNameA(&view->ofn) && !model->str)
        return PreparePrint(model, view, view->ofn.lpstrFile);
    return 1;
}

void Create(TModel* model, TView* view, CREATESTRUCT* cs)
{
    HDC hdc = GetDC(view->hwnd);
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    ReleaseDC(view->hwnd, hdc);

    FillMetrics(view);
    InitDialog(view);

    if (!PreparePrint(model, view, (char*)cs->lpCreateParams))
        {
            HMENU hMenu = GetMenu(view->hwnd);
            EnableMenuItem(hMenu, IDM_OPEN, MF_GRAYED);
            EnableMenuItem(hMenu, IDM_CLOSE, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_VIEW, MF_ENABLED | MF_BYPOSITION);
        }
}

void MenuProcessing(TModel* model, TView* view, WPARAM wParam)
{
    HMENU hMenu = GetMenu(view->hwnd);

    switch(LOWORD(wParam))
    {
    case IDM_OPEN:

        if (!OpenNewFile(model, view))
            {
                EnableMenuItem(hMenu, IDM_OPEN, MF_GRAYED);
                EnableMenuItem(hMenu, IDM_CLOSE, MF_ENABLED);
                EnableMenuItem(hMenu, IDM_VIEW, MF_ENABLED | MF_BYPOSITION);
                SendMessage(view->hwnd, WM_SIZE, 0, 0);
            }

        break;
    case IDM_CLOSE:

        ClearModel(model);
        ClearView(view);
        EnableMenuItem(hMenu, IDM_OPEN, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_CLOSE, MF_GRAYED);
        EnableMenuItem(hMenu, IDM_VIEW, MF_GRAYED | MF_BYPOSITION);
        SendMessage(view->hwnd, WM_SIZE, 0, 0);

        break;
    case IDM_EXIT:

        SendMessage(view->hwnd, WM_DESTROY, 0, 0L);

        break;

    case IDM_SIMPLE:

        view->mode = IDM_SIMPLE;
        ChangeMode(model, view);
        CheckMenuItem(hMenu, IDM_SIMPLE, MF_CHECKED);
        CheckMenuItem(hMenu, IDM_LAYOUT, MF_UNCHECKED);
        EnableMenuItem(hMenu, IDM_LAYOUT, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_SIMPLE, MF_GRAYED);
        SendMessage(view->hwnd, WM_SIZE, 0, 0);

        break;
    case IDM_LAYOUT:

        view->mode = IDM_LAYOUT;
        CheckMenuItem(hMenu, IDM_LAYOUT, MF_CHECKED);
        CheckMenuItem(hMenu, IDM_SIMPLE, MF_UNCHECKED);
        EnableMenuItem(hMenu, IDM_SIMPLE, MF_ENABLED);
        EnableMenuItem(hMenu, IDM_LAYOUT, MF_GRAYED);
        SendMessage(view->hwnd, WM_SIZE, 0, 0);
        ChangeMode(model, view);
        SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);

        break;
    }
}
