#include "TModel.h"
#include "TView.h"

void ChangeVertSize(TView* view, TModel* model, LPARAM lParam)
{
    view->heightWnd = HIWORD(lParam);
    view->countLines = view->heightWnd / view->heightChar;
    view->lastVertPos = max(0, model->sizeOffset - 1 - view->countLines);
    view->vertScrollPos = min(view->vertScrollPos, view->lastVertPos);

    SetScrollRange(view->hwnd, SB_VERT, 0, view->lastVertPos, FALSE);
    SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);
}

void ChangeHorzSize(TView* view, TModel* model, LPARAM lParam)
{
    view->widthWnd = LOWORD(lParam);
    view->countChars = view->widthWnd / view->widthChar - 1;
    view->lastHorzPos = max(0, model->maxLine / view->widthChar - view->countChars + 1);
    view->horzScrollPos = min(view->horzScrollPos, view->lastHorzPos);

    SetScrollRange(view->hwnd, SB_HORZ, 0, view->lastHorzPos, FALSE);
    SetScrollPos(view->hwnd, SB_HORZ, view->horzScrollPos, TRUE);
}
