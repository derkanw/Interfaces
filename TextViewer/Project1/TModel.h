#pragma once

typedef struct
{
    char* str; //����� ��� ������
    unsigned long length; //����� �����
    unsigned int maxLine; //������������ ����� ������ � ��������

    unsigned int* offset; //������ ���������� �������� ������
    unsigned int sizeOffset; //������ ������� ��������
} TModel;

/**
* ����� �������� � ������������� ����� ��������� ������
* ������������ ��� �������� ���� � ����������� WM_CREATE
* @PARAM[OUT] - ��������� �� ������������������ ��������� ������
*/
TModel* InitModel(void);

/**
* ����� ���������� ������� �������� ������
* ������������ ��� ����������� ������ ������ � ���� � ������ ��� �������
* @PARAM[IN] - ��������� �� ��������� ������ ��� ������������� ��������������� �� �����
*/
void FillOffset(TModel* model);

/**
* ����� �������� ����� ��������� ������ � ������� �� ��������� ��������
* ������������ � ������������� ������, �������� ���������� ����� � �������� ���������
* @PARAM[IN] - ��������� �� ��������� ������ ��� ������������� ��������������� �� �����
*/
void ClearModel(TModel* model);

/**
* ����� �������� ��������� � ��������������� ��������� ��������������� �����
* ������������ � ���������� WM_DESTROY
* @PARAM[IN] - ��������� �� ��������� ������ ��� ������������� ��������������� �� �����
*/
void DeleteModel(TModel* model);