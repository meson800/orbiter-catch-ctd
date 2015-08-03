//Copyright (c) 2015 Christopher Johnstone(meson800) and Benedict Haefeli(jedidia)
//The MIT License - See ../../LICENSE for more info
#pragma once

#include <string>
#include <iostream>
#include <fstream>

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

    template<typename ... Types>
    //public case, opens so we only need one open per write
    static void writeToLog(Types ... rest)
    {
        logFile = std::ofstream("./catch-ctd.log", std::ios::app);
        writeToLogOstream(rest...);
        logFile.close();
    }

private:
    template<typename ... Types>
    //Generic case, prints out using << operator
    static void writeToLogOstream(IndentString first, Types ... rest)
    {
        for (int i = 0; i < indent; ++i)
        {
            logFile << "\t";
        }
        //recurse for other arguments
        writeToLogOstream(rest...);
    }

    template<typename T, typename ... Types>
    //Generic case, prints out using << operator
    static void writeToLogOstream(T first, Types ... rest)
    {
        logFile << first;
        //recurse for other arguments
        writeToLogOstream(rest...);
    }
    //base case, write out newline
    static std::ofstream logFile;
    static void writeToLogOstream();
    static int indent;
};
