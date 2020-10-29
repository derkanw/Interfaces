#pragma once

typedef struct
{
    char* str; //����� ��� ������
    unsigned long length; //����� �����
    unsigned int maxLine; //������������ ����� ������ � ��������

    unsigned int* offset; //������ ���������� �������� ������
    unsigned int sizeOffset; //��������� ������ ������� ��������
} TModel;

TModel* InitModel(char* filename); //�����,���������������� ���� ������ ��������� � ���������� ������ �� �����
void FillOffset(TModel* model); //���������� ������� ��������, ������������ ��� ����������� ������ �� �����
void DeleteModel(TModel* model); //������������ ������������ ��������