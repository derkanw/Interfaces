#pragma once

typedef struct
{
    char* str; //����� ��� ������
    unsigned long length; //����� �����
    unsigned int maxLine; //������������ ����� ������ � ��������

    unsigned int* offset; //������ ���������� �������� ������
    unsigned int sizeOffset; //��������� ������ ������� ��������
} TModel;

TModel* InitModel(void); //�����,���������������� ���� ������ ��������� � ���������� ������ �� �����
int FillModel(TModel* model, char* filename);
void FillOffset(TModel* model); //���������� ������� ��������, ������������ ��� ����������� ������ �� �����
void ClearModel(TModel* model);
void DeleteModel(TModel* model); //������������ ������������ ��������
