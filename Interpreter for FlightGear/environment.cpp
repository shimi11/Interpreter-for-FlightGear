#include"environment.h"


Environment* Environment::instance = 0;

Environment* Environment::getInstance()
{
	if (instance == 0)
	{
		instance = new Environment();
	}

	return instance;
}

Environment::Environment()
{
	Command* server = new ServerCommand();
	Command* client = new ClientCommand();

	command_map.insert({ "openDataServer", server });

	command_map.insert({ "connect", client });


	Command* var = new VarCommand();
	command_map.insert({ "var",  var });

	Command* print = new PrintCommand();
	command_map.insert({ "print",  print });

	Command* sleep = new SleepCommand();
	command_map.insert({ "sleep",  sleep });

	Command* send = new SendCommand();
	command_map.insert({ "=",  send });

	command_map.insert({ "log",  new LogCommand() });

}

Environment::~Environment()
{
	unordered_map<string, Command*>::iterator itr;
	for (itr = command_map.begin(); itr != command_map.end(); ++itr)
	{
		delete itr->second;		
	}
}

Command* Environment::getCommand(string str) {
	if (command_map.find(str) != command_map.end())
		return command_map[str];
	return NULL;
}
