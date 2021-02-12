#include "changingSize.h"
#include "supporting.h"

void ChangeVertSize(TModel* model, TView* view)
{
    RECT rc;
    GetClientRect(view->hwnd, &rc);
    view->heightWnd = rc.bottom - rc.top;

    view->countLines = view->heightWnd / view->heightChar;

    if (view->mode == IDM_LAYOUT)
        view->lastVertPos = max(0, (int)view->sizeLayoutOffset - 1 - (int)view->countLines);
    else
        view->lastVertPos = max(0, (int)model->sizeOffset - 1 - (int)view->countLines);

    unsigned int remainder = view->lastVertPos % view->sizeVertScroll;
        view->lastVertPos /= view->sizeVertScroll;
        if (remainder != 0)
            ++view->lastVertPos;

    if (view->mode == IDM_LAYOUT)
    {
        view->vertScrollPos = GetSameString(view->currentString, view->layoutOffset,
                                            view->sizeLayoutOffset);
        CorrectPos(view);
    }

    view->vertScrollPos = min(view->vertScrollPos, view->lastVertPos);
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
            view->currentString = view->layoutOffset[view->vertScrollPos * view->sizeVertScroll];
            ClearView(view);
            if (model->str)
                LayoutMode(model, view);
        }
    else
        view->lastHorzPos = max(0, (int)model->maxLine - (int)view->countChars - 2);

    view->horzScrollPos = min(view->horzScrollPos, view->lastHorzPos);

    SetScrollRange(view->hwnd, SB_HORZ, 0, view->lastHorzPos, FALSE);
    SetScrollPos(view->hwnd, SB_HORZ, view->horzScrollPos, TRUE);
}
