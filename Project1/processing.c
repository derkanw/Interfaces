#include "TModel.h"
#include "TView.h"

char* SelectLine(char* str, unsigned int from, unsigned int to)
{
    unsigned sizeLine = to - from + 1;
    char* temp = (char*)malloc(sizeof(char) * sizeLine);
    if (!temp)
        return NULL;

    for (int i = 0; i < sizeLine; i++)
        temp[i] = str[i + from];

    temp[sizeLine - 1] = '\0';

    return temp;
}

void MaxLineLength(TModel* model, TView* view)
{
    HDC hdc = GetDC(view->hwnd);
    SIZE sz;
    model->maxLine = 0;

    for (unsigned int i = 1; i < model->sizeOffset; i++)
    {
        unsigned int sizeLine = model->offset[i] - model->offset[i - 1];

        GetTextExtentPoint32A(hdc, SelectLine(model->str, model->offset[i - 1], model->offset[i]), sizeLine, &sz);
        if (model->maxLine < sz.cx)
            model->maxLine = sz.cx;
    }

    ReleaseDC(view->hwnd, hdc);
}

int PreparePrint(TModel* model, TView* view, char* filename)
{
    if (FillModel(model, filename))
        return 1;
    FillOffset(model);
    MaxLineLength(model, view);
    view->sizeVertScroll = (model->sizeOffset - 1) / (2 * MAXSHORT) + 1;
    view->sizeHorzScroll = model->maxLine / (2 * MAXSHORT) + 1;
    return 0;
}

int OpenNewFile(TModel* model, TView* view)
{
    if (GetOpenFileNameA(&view->ofn) && !model->str)
        return PreparePrint(model, view, view->ofn.lpstrFile);
}

void PrintText(TModel* model, TView* view)
{
    if (!model->str)
        return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(view->hwnd, &ps);
    int beginPaint = 0, endPaint = model->sizeOffset - 1;
    if (view->vertScrollPos != 0)
    {
        beginPaint = max(0, view->vertScrollPos + ps.rcPaint.top / view->heightChar - 1);
        endPaint = min(model->sizeOffset - 1, view->vertScrollPos + ps.rcPaint.bottom / view->heightChar + 1);
    }

    unsigned int newX = 0, newY = 0;

    for (unsigned int i = beginPaint; i < endPaint; i++)
    {
        char* temp = SelectLine(model->str, model->offset[i], model->offset[i + 1]);
        newX = view->widthChar * (1 - view->horzScrollPos);
        newY = view->heightChar * (i - view->vertScrollPos);
        TextOut(hdc, newX, newY, temp, strlen(temp));
        free(temp);
    }
    EndPaint(view->hwnd, &ps);
}
