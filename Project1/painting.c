#include "painting.h"
#include "supporting.h"

void PrintText(HDC hdc, TModel* model, TView* view, unsigned int* offset, unsigned int sizeOffset,
               PAINTSTRUCT ps)
{
    int beginPaint = max(0, (int)(view->vertScrollPos + ps.rcPaint.top / view->heightChar) - 1);
    int endPaint = min(sizeOffset, view->vertScrollPos + ps.rcPaint.bottom / view->heightChar);

    unsigned int newX = 0, newY = 0;

    for (unsigned int i = beginPaint; i < endPaint; i++)
        {
            newX = view->widthChar * (1 - view->horzScrollPos);
            newY = view->heightChar * (i - view->vertScrollPos);

            char* temp = SelectLine(model->str, offset[i], offset[i + 1]);
            TextOut(hdc, newX, newY, temp, strlen(temp));
        }
}

void Paint(TModel* model, TView* view)
{
    if (!model->str)
        return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(view->hwnd, &ps);

    if (view->mode == IDM_LAYOUT)
        PrintText(hdc, model, view, view->layoutOffset, view->sizeLayoutOffset - 1, ps);
    else
        PrintText(hdc, model, view, model->offset, model->sizeOffset - 1, ps);

    EndPaint(view->hwnd, &ps);
}
