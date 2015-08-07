#include "ManualHooks.h"
#include "detours.h"
#include "Log.h"
#include "VesselProxy.h"

#include <map>

WINAPI_LoadLibraryA pLoadLibraryA = (WINAPI_LoadLibraryA)LoadLibraryA;
OSDK_ovcInit pOvcInit = 0;

FARPROC lastLoadedExit;

std::map<FARPROC,OSDK_ovcExit> hookedExits;

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
    Log::writeToLogDameon(IndentString(), "Detaching LoadLibraryA...\r\n");
    DetourDetach(&(PVOID&)pLoadLibraryA, MyLoadLibraryA);

    //detach all hooked exits
    for (auto it = hookedExits.begin(); it != hookedExits.end(); ++it)
    {
        Log::writeToLogDameon(IndentString(), "Detaching ovcExit:", it->second, "\r\n");
        DetourDetach(&(PVOID&)it->second, MyOvcExit);
    }

    return DetourTransactionCommit();
}

HMODULE _stdcall MyLoadLibraryA(LPCSTR libFileName)
{
    Log::writeToLogDameon(IndentString(), "LoadLibraryA:", "(libFileName:", libFileName, ")\r\n");
    Log::increaseIndent();
    HMODULE result = pLoadLibraryA(libFileName);

    //check if this is a vessel module
    OSDK_ovcInit newInit = (OSDK_ovcInit)GetProcAddress(result, "ovcInit");
    FARPROC ovcExitEntryPoint = GetProcAddress(result, "ovcExit");
    if (newInit && ovcExitEntryPoint)
    {
        //detach old detour if we have one
        if (pOvcInit)
        {
            Log::writeToLogDameon(IndentString(), "Detaching old vessel ovcInit:", pOvcInit, "\r\n");
            DetourTransactionBegin();
            DetourSetIgnoreTooSmall(true);
            DetourUpdateThread(GetCurrentThread());
            DetourDetach(&(PVOID&)pOvcInit, MyOvcInit);
            DetourTransactionCommit();
        }
        Log::writeToLogDameon(IndentString(), "Attaching hook to vessel DLL:", libFileName, ", original ovcInit:", newInit, "\r\n");
        //attach new pointer
        DetourTransactionBegin();
        DetourSetIgnoreTooSmall(true);
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)newInit, MyOvcInit);
        
        DetourTransactionCommit();
        //and set ovcInit poniter
        pOvcInit = newInit;

        //append exit to map, only if we haven't already hooked it
        //so we know which one to actually call
        if (!hookedExits.count(ovcExitEntryPoint))
        {
            OSDK_ovcExit newExit = (OSDK_ovcExit)ovcExitEntryPoint;
            Log::writeToLogDameon(IndentString(), "Attaching hook to ovcExit:", newExit, "\r\n");
            //current exit hasn't been hooked, hook it
            DetourTransactionBegin();
            DetourSetIgnoreTooSmall(true);
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID&)newExit, MyOvcExit);
            DetourTransactionCommit();

            //now set map correctly
            hookedExits[ovcExitEntryPoint] = newExit;
            lastLoadedExit = ovcExitEntryPoint;
        }
        else
        {
            //we've already hooked this one, just set the correct exit
            lastLoadedExit = ovcExitEntryPoint;
        }
    }

    Log::decreaseIndent();
    Log::writeToLogDameon("...\r\n");
    return result;
}

VESSEL* MyOvcInit(OBJHANDLE hvessel, int flightmodel)
{
    Log::writeToLogDameon(IndentString(), "Inside ovcInit\r\n");
    Log::increaseIndent();
    //get vessel pointer to vessel
    VESSEL* realVessel = pOvcInit(hvessel, flightmodel);
    Log::writeToLogDameon(IndentString(), "Creating vessel proxy for class name:", realVessel->GetClassName(), ", name:", realVessel->GetName(), "\r\n");
    VESSEL* result = 0;
    //find out which vessel version to create
    switch (realVessel->Version())
    {
        //pass the vessel proxy the last loaded exit
    case 0:
        //it's a vessel
        result = new VESSELProxy(realVessel, hookedExits[lastLoadedExit], hvessel, flightmodel);
        break;
    case 1:
        //vessel2
        result = new VESSEL2Proxy((VESSEL2*)realVessel, hookedExits[lastLoadedExit], hvessel, flightmodel);
        break;
    case 2:
        //vessel3
        result = new VESSEL3Proxy((VESSEL3*)realVessel, hookedExits[lastLoadedExit], hvessel, flightmodel);
        break;
    }
    Log::decreaseIndent();
    Log::writeToLogDameon("...\r\n");

    //unhook hook
    Log::writeToLogDameon(IndentString(), "Detaching old vessel ovcInit:", pOvcInit, "\r\n");
    DetourTransactionBegin();
    DetourSetIgnoreTooSmall(true);
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pOvcInit, MyOvcInit);
    DetourTransactionCommit();
    return result;
}

void MyOvcExit(VESSEL* vessel)
{
    if (vessel)
    {
        switch (vessel->Version())
        {
        case 0:
            //it's a vessel
            delete ((VESSELProxy*)vessel);
            break;
        case 1:
            //vessel2
            delete ((VESSEL2Proxy*)vessel);
            break;
        case 2:
            //vessel3
            delete ((VESSEL3Proxy*)vessel);
            break;
        }
    }
}