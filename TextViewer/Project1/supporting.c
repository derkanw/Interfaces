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
    model->maxLine = 0;
    for (unsigned int i = 1; i < model->sizeOffset; i++)
    {
        unsigned long sizeLine = model->offset[i] - model->offset[i - 1];
        if (model->maxLine < sizeLine)
            model->maxLine = sizeLine;
    }
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

void LayoutMode(TModel* model, TView* view)
{
    RECT rc;
    GetClientRect(view->hwnd, &rc);
    unsigned int countChars = (rc.right - rc.left) / view->widthChar - 1;

    for (unsigned int i = 0; i < model->sizeOffset - 1; i++)
    {
        ResizeOffset(&view->layoutOffset, &view->sizeLayoutOffset, model->offset[i]);
        unsigned long sizeLine = model->offset[i + 1] - model->offset[i];
        unsigned int j = 0;

        while (sizeLine > countChars)
        {
            j++;
            unsigned int value = model->offset[i] + j * countChars;
            ResizeOffset(&view->layoutOffset, &view->sizeLayoutOffset, value);
            sizeLine -= countChars;
        }
    }
    view->layoutOffset[view->sizeLayoutOffset - 1] = model->offset[model->sizeOffset - 1];
    view->sizeVertScroll = (model->sizeOffset - 1) / view->maxPhysPos + 1;
    view->sizeHorzScroll = model->maxLine / view->maxPhysPos + 1;

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
        view->lastVertPos = max(0, (int)view->sizeLayoutOffset - 1 - (int)view->countLines);
    }
    else
    {
        index = view->layoutOffset[view->vertScrollPos];
        view->vertScrollPos = GetSameString(index, model->offset, model->sizeOffset);
        view->lastVertPos = max(0, (int)model->sizeOffset - 1 - (int)view->countLines);
    }
}
