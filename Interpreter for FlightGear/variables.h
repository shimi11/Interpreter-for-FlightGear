#pragma once
#include <unordered_map>
#include <iostream>
using namespace std;

class Variables
{
public:
	static Variables* getInstance();
	~Variables();
	string getValueBind(string str);
	float getValue(string str);

	void addToBindMap(string key, string value);
	void addToVarMap(string key, float value);

private:
	Variables();
	static Variables* instance;

	unordered_map<string, float> var_map;
	unordered_map<string, string> bind_map;
};
