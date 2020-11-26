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
* ����� ��������� ����������� ���������� ������� ��������
* ����������� ��� ���������� ��������� �������� ��������� ��������
* ������������ ��� ���������� �������� �������� ��� ������� ������ � �������� � ���
* @PARAM[IN] - ��������� �� ������ �������� � ��� ������ ��� ������������ ���������� ��������, � �����
* ��������� �������� ��� ���������� �������� �������
*/
void ResizeOffset(unsigned int** offset, unsigned int* sizeOffset, unsigned int index);

/**
* ����� ����������� ������������ ���������� �������� ��������� ��������� ������
* ������������ ��� ���������� ������� �������� ��� ������ �������
* ����� ���������� ��������� �������� ��������� ����� ������� ������ �� ����� ��������� � ��������
* ��������� �������� ��� ���������� ��������� ���������
* @PARAM[IN] - ���������� ��������� �����������, �������� ������, ����������� ��������� �����
* ���������, ��������� �� ��������� �������� ���������, ��������� �� ����� �������� ������
* @PARAM[OUT] - ������������ ���������� �������� ��������� ��������� ������
*/
unsigned int MaxLayoutLine(HDC hdc, char* str, unsigned int boarder, unsigned int* beginSubline,
                           unsigned int* sizeLine);

/**
* ����� ���������� ������� �������� ������ ��� ������ � ������ �������
* ������������ ��� �������� ������ �� ����� � ������ � ��������� ������ ���� � ������ �������
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����
*/
void LayoutMode(TModel* model, TView* view);

/**
* ����� ������ ������ ������ � ��������� ������� ��������
* ��������� ������ ������������� �������� �� ���������� �������� ��������� ������
* ������������ ��� ��������� ������ ���� � ������ ������ ������
* @PARAM[IN] - ��������� �������� ��������� ������, ������ �������� ������ � ��� ������
* @PARAM[OUT] - ����� ������ � ��������� ������� ��������
*/
unsigned int GetSameString(unsigned int index, unsigned int* offset, unsigned int sizeOffset);

/**
* ����� ���������� ������� �������� ��� ��������� ������ �����������
* ������������ ��� ��������� ������ ���������������� ������ ����
* @PARAM[IN] - ��������� �� ��������� ������ � ������ � ���� ��� ������������� �������������� �����
*/
void ChangeMode(TModel* model, TView* view);
