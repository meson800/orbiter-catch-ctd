#ifndef CATCH_CTD_MANUAL_HOOKS
#define CATCH_CTD_MANUAL_HOOKS

#include <Windows.h>

LONG AttachManualHooks();
LONG DetachManualHooks();

typedef HMODULE(_stdcall *WINAPI_LoadLibraryA)(LPCSTR);
HMODULE _stdcall MyLoadLibraryA(LPCSTR libFileName);

#endif