#include "log.h"




Log::Log()
{
    createAndGetLogP(ERR_LOG);
}

Log* Log::obj = 0;
Log* Log::getInstance()
{

    if (obj == nullptr)
    {
        obj = new Log();
    }
    return obj;
}

Log::~Log()
{
    logsMap.clear();
}


void Log::printToLog(string logName, string toprint) {

    ofstream file; 
    file.open(PATH_LOGS + createAndGetLogP(logName), std::fstream::in | std::fstream::out | std::fstream::app);
    if (file.is_open())
    {
        file << toprint << "\n";
        file.close();
    }
}

void Log::printToErrorLog(string toprint)
{
    printToLog(ERR_LOG, toprint);
}



string Log::createAndGetLogP(string logName)
{
    if (logsMap.find(logName) == logsMap.end())
        logsMap.insert({ logName, logName+ '_' + currentDateTime() + ".log"});
    return logsMap[logName];
}

string  Log::currentDateTime() {

    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
#pragma warning(suppress : 4996)
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y_%m_%d %H_%M_%S", &tstruct);

    return buf;
}