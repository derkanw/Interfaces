#pragma once
#include "TModel.h"
#include "TView.h"

void PrintText(HDC hdc, TModel* model, TView* view, unsigned int* offset, unsigned int endPaint);
void Paint(TModel* model, TView* view);
