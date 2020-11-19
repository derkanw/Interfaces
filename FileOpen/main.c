#include <stdio.h>
#include "main.h"

void pup()
{
    printf("1");
}
/*int FillModel(char* str, unsigned long* length, char* filename) //метод,инициализирующий поля данной структуры и считывание текста из файла
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return 1;

    fseek(f, 0, SEEK_END);
    (*length) = ftell(f);

    str = (char*)malloc(sizeof(char) * (*length));
    if (!str)
        return 1;

    fseek(f, 0, SEEK_SET);
    if ((*length) != fread(str, sizeof(char), (*length), f))
        return 1;

    fclose(f);

    str[(*length)] = '\0';
    return 0;
}*/
