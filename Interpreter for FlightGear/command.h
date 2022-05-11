#pragma once

#include <iostream>
//#include <sstream> 

using namespace std;
#include "variables.h"
#include "DATA.h"
#include "communication.h"
#include "log.h"
#include "Calculate.h"





class Command
{
public:
	virtual void Do_Command(list<string> line) = 0;
	//virtual ~Command() {};
};



class ServerCommand : public Command
{
public:
	ServerCommand() {}
	virtual ~ServerCommand() {};

	void Do_Command(list<string> line);
private:
};

class ClientCommand : public Command
{
public:
	ClientCommand()  {}
	virtual ~ClientCommand() {};
	void Do_Command(list<string> line);
private:

};

class SendCommand : public Command 
{
public:
	SendCommand()  {}
	virtual ~SendCommand() {};

	void Do_Command(list<string> line);
private:
	
};

class VarCommand : public Command
{
public:
	VarCommand() {}
	virtual ~VarCommand() {};

	void Do_Command(list<string> line);
private:

};

class PrintCommand : public Command
{
public:
	PrintCommand()  {}
	virtual ~PrintCommand() {};

	void Do_Command(list<string> line);
private:

};

class LogCommand : public Command
{
public:
	LogCommand() {}
	virtual ~LogCommand() {};

	void Do_Command(list<string> line);
private:

};

class SleepCommand : public Command
{
public:
	SleepCommand()  {}
	virtual ~SleepCommand() {};

	void Do_Command(list<string> line);
private:
	

};