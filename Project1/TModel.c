#include <stdlib.h>
#include "TModel.h"
#include "supporting.h"

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

void FillOffset(TModel* model)
{
    for (unsigned int i = 0; i < model->length; i++)
        if (model->str[i] == '\n')
            ResizeOffset(&model->offset, &model->sizeOffset, i);

    model->offset[model->sizeOffset - 1] = model->length;
}

void DeleteModel(TModel* model)
{
    ClearModel(model);
    if (model->offset)
        free(model->offset);
    free(model);
}
