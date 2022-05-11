#pragma once
#include <unordered_map>
#include <iostream>
using namespace std;

#include "command.h"



class Environment
{
public:
	static Environment* getInstance();
	~Environment();
	Command* getCommand(string str);

private:
	Environment();
	static Environment* instance;
	unordered_map<string, Command*> command_map;
};
