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
    unsigned int* layoutOffset; //массив построчных смещений текста
    unsigned int sizeLayoutOffset; //начальный размер массива смещений

    BOOL layout; //флаг верстки
    HWND hwnd; //дескриптор окна
    OPENFILENAME ofn;
    char filename[MAX_PATH];
} TView;

TView* InitView(HWND classHwnd); //инициализация структуры
void InitDialog(TView* view);
void FillMetrics(TView* view); //заполнение метрик символа, используется для вычисления полей относительной метрики
void ClearView(TView* view);
void DeleteView(TView* view); //освобождение структуры
