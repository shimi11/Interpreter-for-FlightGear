#pragma once
#include <iostream>
#include <string>
#include <shlobj_core.h>
#include <fstream>
#include <unordered_map>

#define DEBUG

#define PATH_LOGS "logs/"
#define ERR_LOG "Errors"

using namespace std;


class Log
{
public:
	static Log* getInstance();
	
	~Log();
	void printToLog(string logName,string toprint);
	void printToErrorLog(string toprint);
private:
	Log();
	static Log* obj;
	string createAndGetLogP(string logName);
	string currentDateTime();

	unordered_map<string, string> logsMap;
};
