#pragma once
#include "TModel.h"
#include "TView.h"

void ChangeVertSize(TView* view, TModel* model, LPARAM lParam);
void ChangeHorzSize(TView* view, TModel* model, LPARAM lParam);
void ChangeMode(TModel* model, TView* view);
unsigned int GetSameString(unsigned int index, unsigned int* offset, unsigned int sizeOffset);
