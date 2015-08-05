//Copyright (c) 2015 Christopher Johnstone(meson800) and Benedict Haefeli(jedidia)
//The MIT License - See ../../LICENSE for more info
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Orbitersdk.h"

#undef DEBUG

struct IndentString {};

//cout overloads
std::ostream& operator<<(std::ostream& os, const MFDMODESPEC& ms);
std::ostream& operator<<(std::ostream& os, const MFDMODESPECEX& ms);
std::ostream& operator<<(std::ostream& os, const MFDSPEC& ms);
std::ostream& operator<<(std::ostream& os, const RECT& rect);
std::ostream& operator<<(std::ostream& os, const VECTOR3& vec);
std::ostream& operator<<(std::ostream& os, const VESSELSTATUS& vs);

class Log
{
public:
    static void clearLog();
    static void increaseIndent();
    static void decreaseIndent();
    static bool startDameon();
    static bool stopDameon();

    template<typename ... Types>
    //public case, opens so we only need one open per write
    static void writeToLog(Types ... rest)
    {
        std::ofstream file("catch-ctd\\catch-ctd.log", std::ios::app);
        logFile = &file;
        writeToLogOstream(rest...);
        logFile = 0;
        file.close();
    }

    template<typename ... Types>
    static void writeToLogDameon(Types ... rest)
    {
        std::ostringstream logString;
        logFile = &logString;
        //recursively generate log string
        writeToLogOstream(rest...);
        logFile = 0; //reset pointer for next attempt

        //and send it through the dameon
        std::string result = logString.str();
        DWORD bytesWritten;
        WriteFile(hWritePipe, (LPCVOID)result.c_str(), result.size(),&bytesWritten,NULL);
        
    }

private:
    template<typename ... Types>
    //Generic case, prints out using << operator
    static void writeToLogOstream(IndentString first, Types ... rest)
    {
        if (logFile)
        {
            for (int i = 0; i < indent; ++i)
            {
                (*logFile) << "\t";
            }
            //recurse for other arguments
            writeToLogOstream(rest...);
        }
    }

    template<typename T, typename ... Types>
    //Generic case, prints out using << operator
    static void writeToLogOstream(T first, Types ... rest)
    {
        if (logFile)
        {
            (*logFile) << first;
            //recurse for other arguments
            writeToLogOstream(rest...);
        }
    }
    //base case, write out newline
    static std::ostream* logFile;

    //variables
    static HANDLE hWritePipe, hReadPipe;
    static void writeToLogOstream();
    static int indent;
};
