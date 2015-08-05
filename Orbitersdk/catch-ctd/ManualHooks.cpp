#include "ManualHooks.h"
#include "detours.h"
#include "Log.h"

WINAPI_LoadLibraryA pLoadLibraryA = (WINAPI_LoadLibraryA)LoadLibraryA;

LONG AttachManualHooks()
{
    DetourTransactionBegin();
    DetourSetIgnoreTooSmall(true);
    DetourUpdateThread(GetCurrentThread());
    PVOID* currentPointer = 0;
    currentPointer = &(PVOID&)pLoadLibraryA;
    Log::writeToLogDameon("Original LoadLibraryA :", currentPointer, "\r\n");
    DetourAttach(currentPointer, MyLoadLibraryA);

    return DetourTransactionCommit();
}

LONG DetachManualHooks()
{
    DetourTransactionBegin();
    DetourSetIgnoreTooSmall(true);
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pLoadLibraryA, MyLoadLibraryA);

    return DetourTransactionCommit();
}

HMODULE _stdcall MyLoadLibraryA(LPCSTR libFileName)
{
    Log::writeToLogDameon(IndentString(), "LoadLibraryA:", "(libFileName:", libFileName, ")");
    Log::increaseIndent();
    HMODULE result = pLoadLibraryA(libFileName);
    Log::decreaseIndent();
    Log::writeToLogDameon("...\r\n");
    return result;
}