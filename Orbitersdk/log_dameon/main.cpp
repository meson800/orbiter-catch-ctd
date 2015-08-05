#include <Windows.h>
#include <cstdio>
#include <iostream>

int main(int argc, char *argv[])
{
    HANDLE readFile = 0;
    if (argc < 1)
    {
        std::cout << "Not enough args";
    }
    else
    {
        sscanf_s(argv[0], "%p", &readFile);
        std::cout << "Read file handle:" << readFile << "\n";
    }
    char buffer[1024];

    DWORD bytesRead;
    
    HANDLE hLogFile = CreateFile(".\\catch-ctd\\catch-ctd.log", FILE_GENERIC_WRITE | FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(hLogFile, 0, 0, FILE_END);
    while (ReadFile(readFile, buffer, 1024, &bytesRead,NULL))
    {
        DWORD bytesWritten;
        WriteFile(hLogFile, buffer, bytesRead,&bytesWritten,NULL);
    }
    LONG error = GetLastError();
    if (error == 109L)
    {
        std::cout << "Pipe closed, exiting\n";
    }
    else
    {
        std::cout << "Error:" << error << ", exiting\n";
    }
    CloseHandle(hLogFile);
    return 0;
}