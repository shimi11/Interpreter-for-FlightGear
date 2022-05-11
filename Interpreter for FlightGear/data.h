#pragma once
#include <unordered_map>
#include <string>
#include <mutex>
#include <queue>
#include <sstream> 
#include <iostream>
#include <fstream>

using namespace std;

#define FILENAME "generic_small.xml"
#define PATH "C:/Program Files/FlightGear 2020.3/data/Protocol/"


class Data
{
public:
	static Data* getInstance();
	
	~Data();
	bool updateBaf(const char* baf,int bytesIn);
	float getVelue(string key) ;
private:
	Data();
	static Data* instance;

	mutex lock_flag_data;
	int sizeMap;
	float *dataArr;
	string* keyArr;
	int data_index;

	queue<char> BafQueue;
	int baf_size;
	unordered_map<string, float> data_map;

	bool insertToMap();
	bool Update();

};

