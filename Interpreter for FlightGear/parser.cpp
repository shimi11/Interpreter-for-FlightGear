#include "parser.h"
#include "Calculate.h"


bool Parser::parserAllLine()
{

	int size = readyLines.size();
	for (int i = 0; i < size;)
	{
		try {
			
			int lines = parserline(i);
			i += lines;
			
		}
		catch (string e) {
			i += 1;
			e += "in line " + to_string(i);
			Log::getInstance()->printToErrorLog(e);
		}
		catch (...) {
			i += 1;
			Log::getInstance()->printToErrorLog("Parser error: in line "+to_string(i));
		}
		
	}
	return 0;
}

int Parser::parserline(int line_number)
{
	int n_parsed = 1;
	list<string> line = readyLines[line_number];

	if (line.empty())
	{
		return n_parsed;//Returns 1 to advance line
	}
	string command_name = line.front();
	line.pop_front();
	if (command_name == "while" || command_name == "if")
	{
		CalculateValue* calculate_value = CalculateValue::getInstance();

		//int lines = 1;
		int next_line = line_number + 1;
		int Curly_brackets = 1;
		while (Curly_brackets)
		{
			if (next_line >= readyLines.size())
			{
				throw (string)"Parser error: Missing flow control bracket '}', starting ";
			}
			if (readyLines[next_line].front()== "while" || readyLines[next_line].front() == "if")
			{
				Curly_brackets++;
			}
			else if (readyLines[next_line].front() == "}")
			{
				Curly_brackets--;
			}
			next_line++;
			n_parsed++;
		}

		if (command_name == "while") {
			while (calculate_value->shuntingYard(&line))
			{
				next_line = line_number + 1;
				while (readyLines[next_line].front() != "}")
				{
					next_line += parserline(next_line);
				}
			}
			return n_parsed;
		}
		else //command_name == "if"
		{
			if (calculate_value->shuntingYard(&line))
			{
				next_line = line_number + 1;
				while (readyLines[next_line].front() != "}")
				{
					next_line += parserline(next_line);
				}
			}
			return n_parsed;
		}
	}
	else
	{
		Environment* allcommand = Environment::getInstance();
		Command* command = allcommand->getCommand(command_name);

		if (command == NULL)
			throw "Parser error: Unknown command. " + command_name;
		command->Do_Command(line);
		
		return n_parsed;
	}

	return n_parsed;//Returns 1 to advance line
}

void Parser::doLexer(string* words) {
	
	if(words[0][0] == '/' && words[0][1] == '/')
		return;
	list<string> line = lexer.tokenize(words);
	if (line.empty())
		return;
	readyLines.push_back(line);
}













