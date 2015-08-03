#define STRICT 1
#define ORBITER_MODULE

#include "Orbitersdk.h"
#include "Hooks.h"
#include "Log.h"

DLLCLBK void InitModule(HINSTANCE hDLL)
{
    Log::clearLog();
    Log::writeToLog("Attaching hooks...\n");
    PVOID* failedPointer;
    LONG result = InstallGlobalOAPIHooks(&failedPointer);
    if (result == NO_ERROR)
    {
        Log::writeToLog("Hooks attached successfully\n");
    }
    else
    {
        Log::writeToLog("Hooks did not attach successfully, error code:", result," , failed pointer:", failedPointer, "\n");
    }
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
    Log::writeToLog("Detaching hooks...\n");
    PVOID* failedPointer;
    LONG result = DetachGlobalOAPIHooks(&failedPointer);
    if (result == NO_ERROR)
    {
        Log::writeToLog("Hooks detached successfully\n");
    }
    else
    {
        Log::writeToLog("Hooks did not detach successfully, error code:", result, " , failed pointer:", failedPointer, "\n");
    }
}
