#define STRICT 1
#define ORBITER_MODULE

#include <Windows.h>
#include "Orbitersdk.h"


HMODULE scenarioEditorDLL;
typedef void(*ExternalInitModule)(HINSTANCE);
typedef void(*ExternalExitModule)(HINSTANCE);
typedef void(*ExternalPause)(bool);
typedef void(*ExternalDeleteVessel)(OBJHANDLE hVessel);
typedef bool(*ExternalDLLMain)(HINSTANCE, DWORD, LPVOID);
typedef OBJHANDLE(*OSDK_GetObjectByName)(char*);
ExternalInitModule scInit;
ExternalExitModule scExit;
ExternalPause scPause;
ExternalDeleteVessel scDeleteVessel;

DLLCLBK void InitModule(HINSTANCE hDLL)
{

    scenarioEditorDLL = LoadLibrary("C:\\Other Stuff\\Orbiter\\catch-ctd\\Modules\\Plugin\\ScnEditor.dll");
    ExternalDLLMain scMain = (ExternalDLLMain)GetProcAddress(scenarioEditorDLL, "DLLMain");
    scInit = (ExternalInitModule)GetProcAddress(scenarioEditorDLL, "InitModule");
    scExit = (ExternalExitModule)GetProcAddress(scenarioEditorDLL, "ExitModule");
    scPause = (ExternalPause)GetProcAddress(scenarioEditorDLL, "opcPause");
    scDeleteVessel = (ExternalDeleteVessel)GetProcAddress(scenarioEditorDLL, "opcDeleteVessel");
    OSDK_GetObjectByName osdkFunc = oapiGetObjectByName;

    scInit(scenarioEditorDLL);
}

DLLCLBK void opcPause(bool pause)
{
    scPause(pause);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
    scExit(hDLL);
}

DLLCLBK void opcDeleteVessel(OBJHANDLE hVessel)
{
    scDeleteVessel(hVessel);
}