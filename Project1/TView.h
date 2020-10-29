#pragma once
#include <Windows.h>

typedef struct
{
    unsigned int heightChar, widthChar; //������� �������
    unsigned int heightWnd, widthWnd; //������� ����
    unsigned int countLines, countChars; //����� �������� � ����� � ����
    unsigned int sizeVertScroll, sizeHorzScroll; //������ ������ �������
    unsigned int lastVertPos, lastHorzPos; //��������� ������� �������
    int vertScrollPos, horzScrollPos; //������� ������� �������

    BOOL layout; //���� �������
    HWND hwnd; //���������� ����
} TView;

TView* InitView(HWND classHwnd); //������������� ���������
void FillMetrics(TView* view); //���������� ������ �������, ������������ ��� ���������� ����� ������������� �������
void DeleteView(TView* view); //������������ ���������
