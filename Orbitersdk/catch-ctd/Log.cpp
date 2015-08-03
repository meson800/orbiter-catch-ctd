//Copyright (c) 2015 Christopher Johnstone(meson800) and Benedict Haefeli(jedidia)
//The MIT License - See ../../LICENSE for more info
#include "Log.h"

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

int Log::indent = 0;
std::ofstream Log::logFile;

void Log::clearLog()
{
    logFile = std::ofstream("./catch-ctd.log", std::ios::out);
    logFile.close();
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
