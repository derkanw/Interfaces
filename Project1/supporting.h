#pragma once
#include "TModel.h"
#include "TView.h"

/**
* ����� ��������� ��������� �� ��������� ������ �� ��������� ���������
* ������������ ��� ������ � ����, ��� ����������� ����������� ���������� ����� ������ ��� ������
* @PARAM[IN] - �������� ������, ��������� � �������� ������� � ������ ��� ��������� ���������
* @PARAM[OUT] - ���������� ���������
*/
char* SelectLine(char* str, unsigned int from, unsigned int to);

/**
* ����� ����������� ������������ ����� ��������� ������
* ���������� ���� ��������� ����������������� ���� ��������� ������
* ������������ ��� �������� ������ �� ����� � ������
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����
*/
void MaxLineLength(TModel* model, TView* view);

/**
* ����� ���������� ������� �������� ������ ��� ������ � ������ �������
* ������������ ��� �������� ������ �� ����� � ������ � ��������� ������ ���� � ������ �������
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����
*/
void LayoutMode(TModel* model, TView* view);
