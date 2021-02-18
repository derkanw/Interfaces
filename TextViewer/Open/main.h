#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <stdio.h>

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

/**
* �����, ����������� �������� ���������� ����� �� ��������� ����� � ������ �� ����
* ������������ ��� �������� ��������������� ����� �� ��������� ������ ��� � ���������� ����
* @Param[IN] char** str - ��������� �� ������ ��������, � ������� ������������ ����� �����
* @Param[IN] unsigned long* length - ��������� �� �����, ������� ����� ��������� ����� �����
* @Param[IN] char* filename - ������ ��������, ���������� �������� ������������ �����
* @Param[OUT] - ��� ���������� ������ ���� unsigned int (0 - �������, 1 - ������)
*/
unsigned int DLL_EXPORT FillModel(char** str, unsigned long* length, char* filename);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
