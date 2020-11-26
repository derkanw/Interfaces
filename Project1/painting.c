#include "painting.h"
#include "supporting.h"

void PrintText(HDC hdc, TModel* model, TView* view, unsigned int* offset, unsigned int endPaint)
{
    unsigned int newX = 0, newY = 0;

    for (unsigned int i = 0; i < endPaint; i++)
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
        PrintText(hdc, model, view, view->layoutOffset, view->sizeLayoutOffset - 1);
    else
    PrintText(hdc, model, view, model->offset, model->sizeOffset - 1);

    EndPaint(view->hwnd, &ps);
}
