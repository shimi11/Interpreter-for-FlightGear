#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <list>

#include "command.h"
#include "environment.h"
#include "lexer.h"


using namespace std;


class Parser 
{
public:
	bool parserAllLine();
	void doLexer(string* words);

private:
	vector <list<string>>readyLines;
	Lexer lexer;
	int parserline(int line);
};

