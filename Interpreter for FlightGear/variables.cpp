#include "variables.h"


Variables* Variables::instance = 0;

Variables* Variables::getInstance()
{
	if (instance == 0)
	{
		instance = new Variables();
	}

	return instance;
}



Variables::Variables()
{
	
}
Variables::~Variables()
{
	
}


void Variables::addToVarMap(string key, float value)
{
	var_map.insert({ key, value });
}

void Variables::addToBindMap(string key, string value)
{
	bind_map.insert({ key, value });
}

string Variables::getValueBind(string str) {
	  
	if (bind_map.find(str) != bind_map.end())
		return bind_map[str];
	return "";
}

float Variables::getValue(string str) {

	if (var_map.find(str) != var_map.end())
		return var_map[str];
	return NULL;
}