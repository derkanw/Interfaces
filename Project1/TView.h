#pragma once
#include <Windows.h>

typedef struct
{
    unsigned int heightChar, widthChar; //размеры символа
    unsigned int heightWnd, widthWnd; //размеры окна
    unsigned int countLines, countChars; //число символов и строк в окне
    unsigned int sizeVertScroll, sizeHorzScroll; //размер одного скролла
    unsigned int lastVertPos, lastHorzPos; //последняя позиция бегунка
    int vertScrollPos, horzScrollPos; //текущая позиция бегунка

    BOOL layout; //флаг верстки
    HWND hwnd; //дескриптор окна
} TView;

TView* InitView(HWND classHwnd); //инициализация структуры
void FillMetrics(TView* view); //заполнение метрик символа, используется для вычисления полей относительной метрики
void DeleteView(TView* view); //освобождение структуры
