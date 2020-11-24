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
* Метод, реализующий загрузку текстового файла по заданному имени и чтение из него
* Используется при открытии соответсвующего файла из командной строки или в диалоговом окне
* @Param[IN] char** str - указатель на буффер символов, в который записывается текст файла
* @Param[IN] unsigned long* length - указатель на число, которое будет содержать длину файла
* @Param[IN] char* filename - массив символов, содержащий название открываемого файла
* @Param[OUT] - код завершения работы типа unsigned int (0 - успешно, 1 - ошибка)
*/
unsigned int DLL_EXPORT FillModel(char** str, unsigned long* length, char* filename);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
