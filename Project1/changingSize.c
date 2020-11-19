#include <stdio.h>
#include "TModel.h"
#include "TView.h"
#include "processing.h"

void ChangeVertSize(TView* view, TModel* model, LPARAM lParam)
{
    if (HIWORD(lParam) && (view->heightWnd != HIWORD(lParam)))
        view->heightWnd = HIWORD(lParam);
    else
        return;

    view->countLines = view->heightWnd / view->heightChar;

    if (view->layout)
    {
        view->lastVertPos = max(0, (int)view->sizeLayoutOffset - 1 - (int)view->countLines);
        for(unsigned int i = 0; i < view->sizeLayoutOffset; i++)
            if (view->layoutOffset[i] == model->offset[view->vertScrollPos])
            {
                view->incModeVertPos = i - view->vertScrollPos;
                break;
            }
        view->vertScrollPos = min(view->vertScrollPos + view->incModeVertPos, view->lastVertPos);
    }
    else
    {
        view->lastVertPos = max(0, (int)model->sizeOffset - 1 - (int)view->countLines);
        view->vertScrollPos = min(view->vertScrollPos - view->incModeVertPos, view->lastVertPos);
    }

    SetScrollRange(view->hwnd, SB_VERT, 0, view->lastVertPos, FALSE);
    SetScrollPos(view->hwnd, SB_VERT, view->vertScrollPos, TRUE);
}

void ChangeHorzSize(TView* view, TModel* model, LPARAM lParam)
{
    if (LOWORD(lParam))
        view->widthWnd = LOWORD(lParam);

    view->countChars = view->widthWnd / view->widthChar - 1;
    if (view->layout)
    {
        view->lastHorzPos = 0;
        ClearView(view);
        LayoutMode(model, view);
    }

    else
        view->lastHorzPos = max(0, (int)(model->maxLine / view->widthChar) - (int)view->countChars + 1);
    view->horzScrollPos = min(view->horzScrollPos, view->lastHorzPos);

    SetScrollRange(view->hwnd, SB_HORZ, 0, view->lastHorzPos, FALSE);
    SetScrollPos(view->hwnd, SB_HORZ, view->horzScrollPos, TRUE);
}
