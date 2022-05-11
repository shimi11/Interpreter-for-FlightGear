#include "parser.h"
#include <fstream>

int main(int argc, char* argv[])
{
	string filename = "Command files\\Command_file.txt";
	
	if (argc > 1) {
		filename = argv[1];
	}
	
	ifstream file;//open_new_file
	file.open(filename);
	if (!file.is_open())
	{
		cout << " the file " << filename << " is not open";
		return 0;
	}
	
	string line;
	list<string> allLines;
	while (getline(file, line))
	{
		if (line.empty())
			continue;
		allLines.push_back(line);
		line.clear();
	}
	if (file.is_open())
		file.close();

	int size = allLines.size();
	Parser parser;
	int i = 0;
	try {
		
		for (; i < size; i++)
		{
			parser.doLexer(&allLines.front());
			allLines.pop_front();
		}

	}
	catch (string e) {
		e += "in line " + to_string(i);
		Log::getInstance()->printToErrorLog(e);
		cout << "Command file not understood, see file errors\n";
		return 0;
	}
	catch (...) {
		Log::getInstance()->printToErrorLog("Lexer error: in line " + to_string(i));
		cout << "Command file not understood, see file errors\n";
		return 0;
	}
	
	parser.parserAllLine();
	
	return 0;
}

