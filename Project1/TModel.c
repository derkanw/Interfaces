#include <stdlib.h>
#include <stdio.h>
#include "TModel.h"

void ClearModel(TModel* model)
{
    if (model->str)
        free(model->str);
    if (model->offset)
        free(model->offset);

    model->str = NULL;
    model->offset = (unsigned int*)malloc(sizeof(unsigned int));
    if (!model->offset)
        return;
    model->offset[0] = 0;
    model->sizeOffset = 2;
    model->maxLine = 0;
}

TModel* InitModel(void)
{
    TModel* model = (TModel*)malloc(sizeof(TModel));
    if (!model)
        return NULL;

    model->str = NULL;
    model->offset = NULL;
    ClearModel(model);

    return model;
}

int FillModel(TModel* model, char* filename) //метод,инициализирующий поля данной структуры и считывание текста из файла
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return 1;

    fseek(f, 0, SEEK_END);
    model->length = ftell(f);

    model->str = (char*)malloc(sizeof(char) * model->length);
    if (!model->str)
        return 1;

    fseek(f, 0, SEEK_SET);
    if (model->length != fread(model->str, sizeof(char), model->length, f))
        return 1;

    fclose(f);

    model->str[model->length] = '\0';
    return 0;
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
    ClearModel(model);
    free(model);
}
