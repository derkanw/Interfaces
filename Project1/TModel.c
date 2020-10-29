#include <stdlib.h>
#include <stdio.h>
#include "TModel.h"

TModel* InitModel(char* filename) //метод,инициализирующий поля данной структуры и считывание текста из файла
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return NULL;

    TModel* model = (TModel*)malloc(sizeof(TModel));
    if (!model)
        return NULL;

    fseek(f, 0, SEEK_END);
    model->length = ftell(f);

    model->str = (char*)malloc(sizeof(char) * model->length);
    if (!model->str)
        return NULL;

    fseek(f, 0, SEEK_SET);
    if (model->length != fread(model->str, sizeof(char), model->length, f))
        return NULL;

    fclose(f);

    model->offset = (unsigned int*)malloc(sizeof(unsigned int));
    if (!model->offset)
        return NULL;

    model->str[model->length] = '\0';
    model->offset[0] = 0;
    model->sizeOffset = 2;

    return model;
}

void FillOffset(TModel* model) //заполнение матрицы смещений, используется для построчного вывода на экран
{
    for (unsigned int i = 0; i < model->length; i++)
        if (model->str[i] == '\n')
            {
                model->sizeOffset++;
                model->offset = (unsigned int*)realloc(model->offset, sizeof(unsigned int) * model->sizeOffset);
                if (!model->offset)
                    return;
                model->offset[model->sizeOffset - 2] = i;
            }
    model->offset[model->sizeOffset - 1] = model->length;
}

void DeleteModel(TModel* model) //освобождение используемой струтуры
{
    free(model->str);
    free(model->offset);
    free(model);
}
