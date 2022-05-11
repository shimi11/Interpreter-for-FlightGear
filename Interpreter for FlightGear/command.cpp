#include "command.h"


void VarCommand::Do_Command(list<string> line) 
{
	string  left_val = line.front();
	line.pop_front();
	string  equal_val = line.front();
	line.pop_front();
	if (equal_val != "=")
		return;
	string right_val = line.front();
	line.pop_front();

	Variables* allVar = Variables::getInstance();
	if (right_val == "bind")
	{
		right_val = line.front();
		line.pop_front();
		allVar->addToBindMap(left_val, right_val);
	}
	else
	{
		Data* data = Data::getInstance();
		CalculateValue* calculate_value = CalculateValue::getInstance();
		//line.clear();
		
		line.push_front(right_val);
		float f = calculate_value->shuntingYard(&line);
		allVar->addToVarMap(left_val, f);
	}
}

void SendCommand::Do_Command(list<string> line) {
	Client* sockts = Client::getInstance();
	string command;
	command.append("set ");

	Variables* allVar = Variables::getInstance();

	string path = allVar->getValueBind(line.front());
	line.pop_front();
	if (path.size() == NULL)
		return;
	

	for(int i = 2 ; (path[i] != 'NULL' && path[i] != NULL);i++)
	{
		if (path[i] == '"')
		{
			continue;
		}
		command.push_back(path[i]);
	}
	command.push_back(' ');
	//*str++;
	CalculateValue* calculate_value = CalculateValue::getInstance();
	float f = calculate_value->shuntingYard(&line);
	
	string str_; 
	stringstream to_string;
	to_string << f;
	to_string >> str_;

	command.append(str_);
	sockts->sendCommend(command);
}

void ServerCommand::Do_Command(list<string> line) {
	stringstream to_int(line.front());
	line.pop_front();
	int myport = 0;
	to_int >> myport;

	stringstream to_int1(line.front());
	line.pop_front();
	int rate = 0;
	to_int1 >> rate;
	Server::getInstance(myport, rate)->startThread();
	//Server *server = new Server(myport, rate);
	//server->startThread();
}

void ClientCommand::Do_Command(list<string> line){
	string ip = line.front();
	line.pop_front();

	string port = line.front();
	line.pop_front();

	stringstream to_int(port);
	int serverport = 0;
	to_int >> serverport;
	Client* client = Client::getInstance(ip.c_str(), serverport);

	//communication->openclient(line.front().c_str(), serverport);
}

void PrintCommand::Do_Command(list<string> line) {

	while (!line.empty())
	{
		string word = line.front();
		if (!word.empty() && word[0] == '\"')
		{
			cout << word;
			line.pop_front();
		}
		else
		{
			list<string> toCalculate;
			while (!word.empty() && word[0] != '\"')
			{
				toCalculate.push_back(word);
				line.pop_front();
				word = (line.empty())?"" : line.front();
			}
			CalculateValue* calculate_value = CalculateValue::getInstance();
			cout << calculate_value->shuntingYard(&toCalculate);

		}
	}
	cout << "\n";


	/*string word = line.front();
	if (!word.empty() && word[0] == '\"')
	{
		size_t length = word.size();
		size_t i = 1;
		for (; i < length && word[i] != '\"'; i++)
		{
			cout << word[i];
		}
		if (i >= length-1 || word[i+1]==' ' || word[i + 1] == '\t')
		{
			cout << endl;
			return;
		}
		line.front() = line.front().substr(i+1);
	}

	CalculateValue* calculate_value = CalculateValue::getInstance();
	float f = calculate_value->shuntingYard(&line);

	cout << f <<"\n";*/
}

void SleepCommand::Do_Command(list<string> line) {
	


	CalculateValue* calculate_value = CalculateValue::getInstance();
	float f = calculate_value->shuntingYard(&line);

	Sleep((int)f);
	
}

void LogCommand::Do_Command(list<string> line)
{
	string logName = line.front();
	line.pop_front();
	string toPrint;
	while (!line.empty())
	{
		string word = line.front();
		if (!word.empty() && word[0] == '\"')
		{
			toPrint.append(word.substr(1, word.size()-2));
			line.pop_front();
		}
		else
		{
			list<string> toCalculate;
			while (!word.empty() && word[0] != '\"')
			{
				toCalculate.push_back(word);
				line.pop_front();
				word = (line.empty()) ? "" : line.front();
			}
			toPrint.append(to_string(CalculateValue::getInstance()->shuntingYard(&toCalculate)));
		}
	}
	Log::getInstance()->printToLog(logName, toPrint);
}
