#include "TView.h"

TView* InitView(HWND classHwnd) //инициализация структуры
{
    TView* view = (TView*)malloc(sizeof(TView));
    if (!view)
        return NULL;

    view->sizeVertScroll = 1;
    view->sizeHorzScroll = 1;
    view->vertScrollPos = 0;
    view->horzScrollPos = 0;
    view->layout = FALSE;

    view->hwnd = classHwnd;

    return view;
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
    free(view);
}
