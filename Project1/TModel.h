#pragma once

typedef struct
{
    char* str; //буфер для текста
    unsigned long length; //длина файла
    unsigned int maxLine; //максимальная длина строки в пикселях

    unsigned int* offset; //массив построчных смещений текста
    unsigned int sizeOffset; //начальный размер массива смещений
} TModel;

TModel* InitModel(void); //метод,инициализирующий поля данной структуры и считывание текста из файла
void FillOffset(TModel* model); //заполнение матрицы смещений, используется для построчного вывода на экран
void ClearModel(TModel* model);
void DeleteModel(TModel* model); //освобождение используемой струтуры
