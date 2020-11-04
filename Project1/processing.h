#pragma once
#include "TModel.h"
#include "TView.h"

char* SelectLine(char* str, unsigned int from, unsigned int to);
void MaxLineLength(TModel* model, TView* view);
int PreparePrint(TModel* model, TView* view, char* filename);
int OpenNewFile(TModel* model, TView* view);
void PrintText(TModel* model, TView* view);
