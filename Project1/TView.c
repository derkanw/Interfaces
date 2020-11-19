#include "TView.h"

void ClearView(TView* view)
{
    if (view->layoutOffset)
        free(view->layoutOffset);

    view->layoutOffset = (unsigned int*)malloc(sizeof(unsigned int));
    if (!view->layoutOffset)
        return;
    view->layoutOffset[0] = 0;
    view->sizeLayoutOffset = 1;
}

TView* InitView(HWND classHwnd) //инициализация структуры
{
    TView* view = (TView*)malloc(sizeof(TView));
    if (!view)
        return NULL;

    view->sizeVertScroll = 1;
    view->sizeHorzScroll = 1;
    view->vertScrollPos = 0;
    view->horzScrollPos = 0;
    view->lastVertPos = 0;
    view->lastHorzPos = 0;
    view->incModeVertPos = 0;
    view->layout = FALSE;
    view->layoutOffset = NULL;

    view->hwnd = classHwnd;

    ClearView(view);
    return view;
}

void InitDialog(TView* view)
{
    ZeroMemory(&view->ofn, sizeof(OPENFILENAME));

    view->ofn.lStructSize = sizeof(OPENFILENAME);
    view->ofn.hwndOwner = view->hwnd;
    view->ofn.lpstrFile = view->filename;
    view->ofn.lpstrFile[0] = '\0';
    view->ofn.nMaxFile = sizeof(view->filename);
    view->ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0";
    view->ofn.nFilterIndex = 1;
    view->ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}

void FillMetrics(TView* view) //заполнение метрик символа, используется для вычисления полей относительной метрики
{
    TEXTMETRIC tm;
    HDC hdc = GetDC(view->hwnd);

    GetTextMetrics(hdc, &tm);
    view->heightChar = tm.tmHeight;
    view->widthChar = tm.tmAveCharWidth;

    ReleaseDC(view->hwnd, hdc);
}

void DeleteView(TView* view) //освобождение структуры
{
    ClearView(view);
    free(view);
}
