#pragma once
#include "lexer.h"

//list<string> Lexer::tokenize(string* words)
//{
//	list<string> line;
//
//	size_t prev = 0, pos;
//	//Regular expression
//	while ((pos = words->find_first_of("=<>!-+*/()\" \t", prev)) != std::string::npos)
//	{
//		if (pos > prev)//Before the character that cuts
//			line.push_back(words->substr(prev, pos - prev));
//
//		if (words->substr(pos, 1) == "\"")//Take the whole phrase
//			break;
//
//		if (words->substr(pos, 1) != " " && words->substr(pos, 1) != "\t")//The character that cut
//			line.push_back(words->substr(pos, 1));
//
//		prev = pos + 1;
//	}
//	if (prev < words->length())//After the character that cut
//		line.push_back(words->substr(prev, std::string::npos));
//
//	if (line.size() <= 2)
//		return line;
//
//
//	list<string>::iterator it, itNext;
//	itNext = it = line.begin();
//	itNext++;
//
//	for (; itNext != line.end(); ++it, ++itNext) {
//		if ((COMPARATIVE_OP(*it)) && *itNext == "=")
//		{
//			string temp = *it;
//			temp.append(*itNext);
//			*it = temp;
//			line.erase(itNext);
//			itNext = it;
//			itNext++;
//		}
//		if ((*it == "=" || *it == "<" || *it == ">" || *it == "("
//			|| *it == "<=" || *it == ">=" || *it == "!=" || *it == "==") && *itNext == "-")
//		{
//			it++;//it == '-'
//			itNext++;// == 'x'
//			string temp = *it;
//			temp.append(*itNext);
//			*it = temp;
//			line.erase(itNext);
//			itNext = it;
//			itNext++;
//		}
//	}
//
//	itNext = it = line.begin();
//	itNext++;
//	if (*itNext == "=")
//	{
//		string temp = *itNext;
//		*itNext = *it;
//		*it = temp;
//	}
//
//	return line;
//}


list<string> Lexer::tokenize(string* words)
{
	list<string> line = maxSplit(words);
	if (line.size() > 2)
	{
		line = unionCharacter(line);
	}
	return line;
}

list<string> Lexer::maxSplit(string* words)
{
	list<string> line;

	size_t prev_ = 0;
	size_t pos = 0;
	size_t LastPos = words->size() - 1;
	//Regular expression
	while ((pos = words->find_first_of("=<>!-+*/()\" \t", prev_)) != std::string::npos)
	{
		if (pos > prev_)//Before the character that cuts
			line.push_back(words->substr(prev_, pos - prev_));

		if (words->substr(pos, 1) == "\"")//Take the whole phrase
		{
			prev_ = pos;
			pos = words->find_first_of("\"", pos +1);
			line.push_back(words->substr(prev_, pos - prev_ +1));
		}
		else if (words->substr(pos, 1) != " " && words->substr(pos, 1) != "\t")//The character that cut
			line.push_back(words->substr(pos, 1));

		prev_ = pos + 1;
		if(prev_ > LastPos+1)
			throw "Lexer error: Unintelligible line ->" + *words +", ";

	}
	if (prev_ < LastPos)//After the character that cut
		line.push_back(words->substr(prev_, std::string::npos));


	return line;
}

list<string> Lexer::unionCharacter(list<string> line)
{
	list<string>::iterator it, itNext,xxxx;
	itNext = it = line.begin();
	itNext++;

	for (;itNext != line.end(); ++it, ++itNext) {
		if ((COMPARATIVE_OP(*it)) && *itNext == "=")
		{
			string temp = *it;
			temp.append(*itNext);
			*it = temp;
			line.erase(itNext);
			itNext = it;
			itNext++;
		}
		if ((*it == "=" || *it == "<" || *it == ">" || *it == "("
			|| *it == "<=" || *it == ">=" || *it == "!=" || *it == "==") && *itNext == "-")
		{
			it++;//it == '-'
			itNext++;// == 'x'
			string temp = *it;
			temp.append(*itNext);
			*it = temp;
			line.erase(itNext);
			itNext = it;
			itNext++;
		}
		if (itNext == line.end())
		{
			break;
		}
	}

	itNext = it = line.begin();
	itNext++;
	if (*itNext == "=")
	{
		string temp = *itNext;
		*itNext = *it;
		*it = temp;
	}

	return line;
}


