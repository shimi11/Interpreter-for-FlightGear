#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include "data.h"
#include "variables.h"

#define BOOLEAN_OP(x) x == "==" || x == "<=" || x == ">=" || x == ">" || x == "<" || x == "!=" 
#define MATHEMATICAL_OP(x) x == "+" || x == "-" || x == "*" || x == "/"

using namespace std;

class CalculateValue
{

public:
    static CalculateValue* getInstance();
    float shuntingYard(list<string> *line);
private:
    static CalculateValue* instance;
    float Calculate(queue< string> queue_);
    bool is_digits(string str);
    bool priorityOp(string op1, string op2);
    CalculateValue();

};




