#include "processing.h"
#include "supporting.h"
#include "../Open/main.h"


int PreparePrint(TModel* model, TView* view, char* filename)
{
    if (FillModel(&model->str, &model->length, filename))
        return 1;

    FillOffset(model);
    MaxLineLength(model, view);
    view->sizeVertScroll = (model->sizeOffset - 1) / (2 * MAXSHORT) + 1;
    view->sizeHorzScroll = model->maxLine / (2 * MAXSHORT) + 1;

    LayoutMode(model, view);

    return 0;
}

int OpenNewFile(TModel* model, TView* view)
{
    if (GetOpenFileNameA(&view->ofn) && !model->str)
        return PreparePrint(model, view, view->ofn.lpstrFile);
    return 1;
}
