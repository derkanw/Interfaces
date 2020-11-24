#include "main.h"

unsigned int DLL_EXPORT FillModel(char** str, unsigned long* length, char* filename)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return 1;

    fseek(f, 0, SEEK_END);
    (*length) = ftell(f);

    (*str) = (char*)malloc(sizeof(char) * (*length));
    if (!(*str))
        return 1;

    fseek(f, 0, SEEK_SET);
    if ((*length) != fread((*str), sizeof(char), (*length), f))
        return 1;

    fclose(f);

    (*str)[(*length)] = '\0';
    return 0;
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
