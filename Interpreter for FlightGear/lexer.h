#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <list>
#include <iterator> 
#include <queue> 
#define COMPARATIVE_OP(x) x == "=" ||x == "<" || x == ">"|| x == "!" 
#define MATHEMATICAL_OP(x) x == "+" || x == "-" || x == "*" || x == "/"

using namespace std;


class Lexer
{
public:
	list<string> tokenize(string* words);// ("x = 5") -> ("x" "=" "5")
private:
	list<string> maxSplit(string* words);
	list<string> unionCharacter(list<string> line);
};












