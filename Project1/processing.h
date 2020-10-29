#pragma once
#include "TModel.h"
#include "TView.h"

char* SelectLine(char* str, unsigned int from, unsigned int to);
void PrintText(TModel* model, TView* view);
void MaxLineLength(TModel* model, TView* view);
