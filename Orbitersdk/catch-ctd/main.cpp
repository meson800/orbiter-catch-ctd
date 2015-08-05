#define STRICT 1
#define ORBITER_MODULE

#include "Orbitersdk.h"
#include "Hooks.h"
#include "Log.h"
#include "ManualHooks.h"

DLLCLBK void InitModule(HINSTANCE hDLL)
{
    Log::clearLog();
    Log::startDameon();
    Log::writeToLogDameon("Attaching hooks...\r\n");
    PVOID* failedPointer;
    LONG result = AttachGlobalOAPIHooks(&failedPointer);
    if (result == NO_ERROR)
    {
        Log::writeToLogDameon("Hooks attached successfully\r\n");
    }
    else
    {
        Log::writeToLogDameon("Hooks did not attach successfully, error code:", result," , failed pointer:", failedPointer, "\r\n");
    }

    Log::writeToLogDameon("Attaching manual hooks...\r\n");
    if (AttachManualHooks() == NO_ERROR)
    {
        Log::writeToLogDameon("Manual hooks attached successfully\r\n");
    }
    else
    {
        Log::writeToLogDameon("Manual hooks did not attach successfully\r\n");
    }
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
    Log::writeToLogDameon("Detaching hooks...\r\n");
    PVOID* failedPointer;
    LONG result = DetachGlobalOAPIHooks(&failedPointer);
    //detachThread.join();
    if (result == NO_ERROR)
    {
        Log::writeToLogDameon("Hooks detached successfully\r\n");
    }
    else
    {
        Log::writeToLogDameon("Hooks did not detach successfully, error code:", result, " , failed pointer:", failedPointer, "\r\n");
    }

    Log::writeToLogDameon("Detaching manual hooks...\r\n");
    if (DetachManualHooks() == NO_ERROR)
    {
        Log::writeToLogDameon("Manual hooks detached successfully\r\n");
    }
    else
    {
        Log::writeToLogDameon("Manual hooks did not detach successfully\r\n");
    }

    Log::stopDameon();
}
