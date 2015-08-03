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
    //Generic case, prints out using << operator
    static void writeToLog(IndentString first, Types ... rest)
    {
        std::ofstream logFile = std::ofstream("./catch-ctd.log", std::ios::app);
        for (int i = 0; i < indent; ++i)
        {
            logFile << "\t";
        }
        logFile.close();
        //recurse for other arguments
        writeToLog(rest...);
    }

    template<typename T, typename ... Types>
    //Generic case, prints out using << operator
    static void writeToLog(T first, Types ... rest)
    {
        std::ofstream logFile = std::ofstream("./catch-ctd.log", std::ios::app);
        logFile << first;
        logFile.close();
        //recurse for other arguments
        writeToLog(rest...);
    }
    //base case, write out newline
    static void writeToLog();
private:
    static int indent;
};
