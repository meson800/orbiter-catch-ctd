#ifndef CATCH_CTD_MANUAL_HOOKS
#define CATCH_CTD_MANUAL_HOOKS
#include "Orbitersdk.h"
#include <Windows.h>


LONG AttachManualHooks();
LONG DetachManualHooks();

typedef HMODULE(_stdcall *WINAPI_LoadLibraryA)(LPCSTR);
HMODULE _stdcall MyLoadLibraryA(LPCSTR libFileName);

typedef VESSEL*(*OSDK_ovcInit)(OBJHANDLE, int);
VESSEL* MyOvcInit(OBJHANDLE hvessel, int flightmodel);

typedef void(*OSDK_ovcExit)(VESSEL*);
void MyOvcExit(VESSEL* vessel);


#endif