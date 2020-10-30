#pragma once
#include "TModel.h"
#include "TView.h"

char* SelectLine(char* str, unsigned int from, unsigned int to);
void MaxLineLength(TModel* model, TView* view);
void OpenNewFile(TModel* model, TView* view);
void PrintText(TModel* model, TView* view);
