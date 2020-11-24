#include "TModel.h"
#include "TView.h"
#include "../Open/main.h"

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

void LayoutMode(TModel* model, TView* view)
{
    SIZE sz;
    HDC hdc = GetDC(view->hwnd);

    RECT rc;
    GetClientRect(view->hwnd, &rc);
    unsigned int widthWnd = rc.right - rc.left;

    for (unsigned int i = 0; i < model->sizeOffset - 1; i++)
    {
        view->sizeLayoutOffset++;
        view->layoutOffset = (unsigned int*)realloc(view->layoutOffset, sizeof(unsigned int) * view->sizeLayoutOffset);
        if (!view->layoutOffset)
            return;
        view->layoutOffset[view->sizeLayoutOffset - 2] = model->offset[i];

        char* temp = SelectLine(model->str, model->offset[i], model->offset[i + 1]);

        GetTextExtentPoint32A(hdc, temp, strlen(temp), &sz);
        unsigned int k = 0;
        unsigned int sizeLine = sz.cx;

        while (sizeLine > widthWnd)
        {
            unsigned int sizeSubline = 0;
            unsigned int j = 0;

            for (j = k; j < strlen(temp); j++)
            {
                char symbol[] = {temp[j]};
                GetTextExtentPoint32A(hdc, symbol, 1, &sz);
                sizeSubline += sz.cx;
                if (sizeSubline > widthWnd - view->widthChar)
                    break;
            }

            view->sizeLayoutOffset++;
            view->layoutOffset = (unsigned int*)realloc(view->layoutOffset, sizeof(unsigned int) * view->sizeLayoutOffset);
            if (!view->layoutOffset)
                return;
            view->layoutOffset[view->sizeLayoutOffset - 2] = model->offset[i] + j;

            sizeLine -= sizeSubline;
            k = j;
        }
    }
    view->layoutOffset[view->sizeLayoutOffset - 1] = model->offset[model->sizeOffset - 1];

    ReleaseDC(view->hwnd, hdc);
}

int PreparePrint(TModel* model, TView* view, char* filename)
{
    if (FillModel(&model->str, &model->length, filename))
        return 1;

    FillOffset(model);
    MaxLineLength(model, view);
    view->sizeVertScroll = (model->sizeOffset - 1) / (2 * MAXSHORT) + 1;
    view->sizeHorzScroll = model->maxLine / (2 * MAXSHORT) + 1;

    LayoutMode(model, view);

    return 0;
}

int OpenNewFile(TModel* model, TView* view)
{
    if (GetOpenFileNameA(&view->ofn) && !model->str)
        return PreparePrint(model, view, view->ofn.lpstrFile);
    return 1;
}

void PrintText(TModel* model, TView* view)
{
    if (!model->str)
        return;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(view->hwnd, &ps);
    unsigned int endPaint;
    if (view->layout)
        endPaint = view->sizeLayoutOffset - 1;
    else
        endPaint = model->sizeOffset - 1;

    unsigned int newX = 0, newY = 0;

    for (unsigned int i = 0; i < endPaint; i++)
    {
        newX = view->widthChar * (1 - view->horzScrollPos);
        newY = view->heightChar * (i - view->vertScrollPos);

        if (view->layout)
        {
            char* temp = SelectLine(model->str, view->layoutOffset[i], view->layoutOffset[i + 1]);
            TextOut(hdc, newX, newY, temp, strlen(temp));
        }
        else
        {
            char* temp = SelectLine(model->str, model->offset[i], model->offset[i + 1]);
            TextOut(hdc, newX, newY, temp, strlen(temp));
        }
    }
    EndPaint(view->hwnd, &ps);
}
