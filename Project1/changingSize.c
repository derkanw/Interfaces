#include "changingSize.h"
#include "supporting.h"

void ChangeVertSize(TModel* model, TView* view)
{
    RECT rc;
    GetClientRect(view->hwnd, &rc);
    view->heightWnd = rc.bottom - rc.top;

    view->countLines = view->heightWnd / view->heightChar;

    if (view->mode == IDM_LAYOUT)
        {
            view->lastVertPos = max(0, (int)view->sizeLayoutOffset - 1 - (int)view->countLines);
            view->vertScrollPos = GetSameString(view->currentString, view->layoutOffset,
                                                view->sizeLayoutOffset);
            view->vertScrollPos = min(view->vertScrollPos, view->lastVertPos);
        }
    else
        {
            view->lastVertPos = max(0, (int)model->sizeOffset - 1 - (int)view->countLines);
            view->vertScrollPos = min(view->vertScrollPos, view->lastVertPos);
        }
    SetScrollRange(view->hwnd, SB_VERT, 0, view->lastVertPos, FALSE);
    SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);
}

void ChangeHorzSize(TModel* model, TView* view)
{
    RECT rc;
    GetClientRect(view->hwnd, &rc);
    view->widthWnd = rc.right - rc.left;

    view->countChars = view->widthWnd / view->widthChar - 1;

    if (view->mode == IDM_LAYOUT)
        {
            view->lastHorzPos = 0;
            view->currentString = view->layoutOffset[view->vertScrollPos];
            ClearView(view);
            if (model->str)
                LayoutMode(model, view);
        }
    else
        view->lastHorzPos = max(0, (int)(model->maxLine / view->widthChar) - (int)view->countChars + 1);

    view->horzScrollPos = min(view->horzScrollPos, view->lastHorzPos);

    SetScrollRange(view->hwnd, SB_HORZ, 0, view->lastHorzPos, FALSE);
    SetScrollPos(view->hwnd, SB_HORZ, view->horzScrollPos, TRUE);
}
