//Copyright (c) 2015 Christopher Johnstone(meson800) and Benedict Haefeli(jedidia)
//The MIT License - See ../../LICENSE for more info
#include "Log.h"
#include <Windows.h>
#include <sstream>
#include <string>

std::ostream& operator<<(std::ostream& os, const MFDMODESPEC& ms)
{
    os << "(key:" << ms.key << ", msgproc:" << ms.msgproc << ", name:" << ms.name << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MFDMODESPECEX& ms)
{
    os << "(context:" << ms.context << ", key:" << ms.key << ", msgproc:" << ms.msgproc << ", name:" << ms.name << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MFDSPEC& ms)
{
    os << "(bt_ydist:" << ms.bt_ydist << ", bt_yofs:" << ms.bt_yofs << ", nbt_left:" << ms.nbt_left 
        << ", nbt_right:" << ms.nbt_right << ", pos:" << ms.pos << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const RECT& rect)
{
    os << "(bottom:" <<rect.bottom << ", left:" << rect.left << ", right:" << rect.right << ", top:" << rect.top << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const VECTOR3& vec)
{
    os << "(data:" << vec.data << ", x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const VESSELSTATUS& vs)
{
    os << "(arot:" << vs.arot << ", base:" << vs.base << ", eng_hovr:" << vs.eng_hovr << ", eng_main:" << vs.eng_main 
        << ", fdata:" << vs.fdata << ", flag:" << vs.flag << ", fuel:" << vs.fuel << ", port:" << vs.port 
        << ", rbody:" << vs.rbody << ", rpos:" << vs.rpos << ", rvel:" << vs.rvel << ", status:" <<vs.status 
        << ", vdata:" << vs.vdata << ", vrot:" << vs.vrot << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const ELEMENTS& em)
{
    os << "(a:" << em.a << ", e:" << em.e << ", i:" << em.i << ", L:" << em.L << 
        ", omegab:" << em.omegab << ", theta:" << em.theta << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MATRIX3& mx)
{
    os << "(data:" << mx.data
        << ", m11:" << mx.m11 << ", m12:" << mx.m12 << ", m13:" << mx.m13
        << ", m21:" << mx.m21 << ", m22:" << mx.m22 << ", m23:" << mx.m23
        << ", m31:" << mx.m31 << ", m32:" << mx.m32 << ", m33:" << mx.m33
        << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MESHGROUP_TRANSFORM& mt)
{
    os << "(ngrp:" << mt.ngrp << ", nmesh:" << mt.nmesh << ", P:";
    switch (mt.transform)
    {
    case MESHGROUP_TRANSFORM::ROTATE:
        os << "(angle:" << mt.P.rotparam.angle << ", axis:" << mt.P.rotparam.axis << ", ref:" << mt.P.rotparam.ref << ")";
        break;
    case MESHGROUP_TRANSFORM::SCALE:
        os << "(scale:" << mt.P.scaleparam.scale << ")";
        break;
    case MESHGROUP_TRANSFORM::TRANSLATE:
        os << "(shift:" << mt.P.transparam.shift << ")";
        break;
    }
    os << ", transform:" << mt.transform << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const COLOUR4& cl)
{
    os << "(a:" << cl.a << ", b:" << cl.b << ", g:" << cl.g << ", r:" << cl.r << ")";
    return os;
}

int Log::indent = 0;
std::ostream* Log::logFile = 0;
HANDLE Log::hWritePipe, Log::hReadPipe;

bool Log::startDameon()
{
    //create anonymous pipe
    if (!CreatePipe(&hReadPipe, &hWritePipe, NULL, 0)) //use default buffer size
    {
        //pipe creation failed
        return false;
    }

    //make pipe inheritable
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);

    //start dameon
    STARTUPINFO         siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

    //make the pointer into an argument
    std::ostringstream arg;
    arg << hReadPipe;
    

    if (!CreateProcess(".\\catch-ctd\\log-dameon.exe",     // Application name
        (LPSTR)(arg.str().c_str()),                 // Application arguments
        0, 0, TRUE, CREATE_DEFAULT_ERROR_MODE, 0, 0, &siStartupInfo, &piProcessInfo))
    {
        //dameon failed to start
        return false;
    }
    return true;
}

bool Log::stopDameon()
{
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    return true;
}

void Log::clearLog()
{
    std::ofstream tempFile("catch-ctd\\catch-ctd.log", std::ios::out);
    tempFile.close();
}

void Log::increaseIndent()
{
    indent++;
}

void Log::decreaseIndent()
{
    indent--;
}

void Log::writeToLogOstream()
{

}
