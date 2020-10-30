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
    OPENFILENAME ofn;
    char filename[MAX_PATH];
} TView;

TView* InitView(HWND classHwnd); //������������� ���������
void InitDialog(TView* view);
void FillMetrics(TView* view); //���������� ������ �������, ������������ ��� ���������� ����� ������������� �������
void DeleteView(TView* view); //������������ ���������
