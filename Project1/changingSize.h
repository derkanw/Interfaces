#pragma once
#include "TModel.h"
#include "TView.h"

void ChangeVertSize(TView* view, TModel* model, LPARAM lParam);
void ChangeHorzSize(TView* view, TModel* model, LPARAM lParam);
void ChangeMode(TModel* model, TView* view);
