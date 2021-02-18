#pragma once
#include "TModel.h"
#include "TView.h"

/**
* �����, ����������� ����������� �������������� ��� �������� ���������� �����
* ���������� ���������� ������� �������, ������ �������� ��� ������� ������, ������� �����������
* ����� ������, ��������� �������� �������� ������� �������� � ����������� �� ������� ������
* ������������ ��� �������� ����� � ��������� ������ ��� ��� ������ �������
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����,
* ��� ��������� ���������� �����
* @PARAM[OUT] - ��� ������ ������ (0 - �������, 1 - ������)
*/
int PreparePrint(TModel* model, TView* view, char* filename);

/**
* ����� �������� ���������� ����� ��� ������ �������
* ������������ ��� ��������� ��������� ���������������� ���� ���� ���������
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����
* @PARAM[OUT] - ��� ������ ������ (0 - �������, 1 - ������)
*/
int OpenNewFile(TModel* model, TView* view);

/**
* ����� ���������� ��������������� ����� ����� �������� ���������� ��������� � ���������� ������� ����
* ������������ ��� ��������� WM_CREATE
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����,
* ��������� �������� ����, �������������� ��� ��������� ����� ���������� ����� � �������
*/
void Create(TModel* model, TView* view, CREATESTRUCT* cs);

/**
* ����� ��������� ��������� ������� ����
* ������������ ��� ��������� WM_COMMAND
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����,
* �������� ��������� ��� ��������� ����
*/
void MenuProcessing(TModel* model, TView* view, WPARAM wParam);