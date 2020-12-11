#include "supporting.h"

char* SelectLine(char* str, unsigned long from, unsigned long to)
{
    unsigned long sizeLine = to - from + 1;
    char* temp = (char*)malloc(sizeof(char) * sizeLine);
    if (!temp)
        return NULL;

    for (unsigned long i = 0; i < sizeLine; i++)
        temp[i] = str[i + from];

    temp[sizeLine - 1] = '\0';

    return temp;
}

void MaxLineLength(TModel* model, TView* view)
{
    SIZE sz;
    HDC hdc = GetDC(view->hwnd);
    model->maxLine = 0;

    for (unsigned int i = 1; i < model->sizeOffset; i++)
    {
        unsigned long sizeLine = model->offset[i] - model->offset[i - 1];
        char* temp = SelectLine(model->str, model->offset[i - 1], model->offset[i]);

        GetTextExtentPoint32A(hdc, temp, sizeLine, &sz);
        if (model->maxLine < sz.cx)
            model->maxLine = sz.cx;
    }

    ReleaseDC(view->hwnd, hdc);
}

void ResizeOffset(unsigned int** offset, unsigned int* sizeOffset, unsigned long index)
{
    (*sizeOffset)++;
    unsigned int size = sizeof(unsigned int) * (*sizeOffset);
    (*offset) = (unsigned int*)realloc((*offset), size);
    if (!(*offset))
        return;
    (*offset)[(*sizeOffset) - 2] = index;
}

unsigned int MaxLayoutLine(HDC hdc, char* str, unsigned int boarder, unsigned int* beginSubline,
                           unsigned int* sizeLine)
{
    SIZE sz;
    unsigned int sizeSubline = 0;

    for (unsigned int i = (*beginSubline); i < strlen(str); i++)
        {
            char symbol[] = {str[i]};
            GetTextExtentPoint32A(hdc, symbol, 1, &sz);
            sizeSubline += sz.cx;
            if (sizeSubline > boarder)
            {
                (*sizeLine) -= sizeSubline;
                (*beginSubline) = i;
                return i;
            }
        }
    return 0;
}

void LayoutMode(TModel* model, TView* view)
{
    SIZE sz;
    RECT rc;
    HDC hdc = GetDC(view->hwnd);

    GetClientRect(view->hwnd, &rc);
    unsigned int widthWnd = rc.right - rc.left, boarder = widthWnd - view->widthChar;

    for (unsigned int i = 0; i < model->sizeOffset - 1; i++)
    {
        ResizeOffset(&view->layoutOffset, &view->sizeLayoutOffset, model->offset[i]);

        char* temp = SelectLine(model->str, model->offset[i], model->offset[i + 1]);
        GetTextExtentPoint32A(hdc, temp, strlen(temp), &sz);

        unsigned int beginSubline = 0, sizeLine = sz.cx;

        while (sizeLine > widthWnd)
        {
            unsigned int value = MaxLayoutLine(hdc, temp, boarder, &beginSubline, &sizeLine);
            ResizeOffset(&view->layoutOffset, &view->sizeLayoutOffset, model->offset[i] + value);
        }
    }
    view->layoutOffset[view->sizeLayoutOffset - 1] = model->offset[model->sizeOffset - 1];

    ReleaseDC(view->hwnd, hdc);
}

unsigned int GetSameString(unsigned long index, unsigned int* offset, unsigned int sizeOffset)
{
    for(unsigned int i = 0; i < sizeOffset - 1; i++)
        if (index >= offset[i] && index < offset[i + 1])
            return i;
    return 0;
}

void ChangeMode(TModel* model, TView* view)
{
    unsigned int index;

    if (view->mode == IDM_LAYOUT)
    {
        index = model->offset[view->vertScrollPos];
        view->vertScrollPos = GetSameString(index, view->layoutOffset, view->sizeLayoutOffset);
    }
    else
    {
        index = view->layoutOffset[view->vertScrollPos];
        view->vertScrollPos = GetSameString(index, model->offset, model->sizeOffset);
    }
}
