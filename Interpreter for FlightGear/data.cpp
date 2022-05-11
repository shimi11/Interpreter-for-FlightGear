#include "data.h"


Data* Data::instance = 0;

Data* Data::getInstance()
{
	if (instance == 0)
	{
		instance = new Data();
	}

	return instance;
}

Data::Data()
{
	baf_size = 0;
	data_index = 0; 
	
	ifstream file;
	string fileName = PATH;
	fileName += FILENAME;
	file.open(fileName);
	if (!file.is_open())
	{
		cout << " the file " << fileName << " is not open";
		return ;
	}
	string line;
	list<string> allLines;

	ostringstream fileContentsStream;
	fileContentsStream << file.rdbuf(); 
	string fileContents = fileContentsStream.str();

	queue < string >keyList;
	string  start = "<node>";
	string  end = "</node>";
	size_t found , foundEnd = 0;
	while (1)
	{
		found = fileContents.find(start, foundEnd+1);
		if (found == string::npos)
			break;
		foundEnd = fileContents.find(end, found + 1);
		string key = fileContents.substr(found + start.size(), foundEnd - (found + start.size()));
		keyList.push(key);
	} 
	
	sizeMap = keyList.size();
	dataArr = new float[sizeMap];
	if (dataArr == NULL)
		Data::~Data();
	keyArr = new string[sizeMap];
	if (keyArr == NULL)
		Data::~Data();

	memset(dataArr, 0, sizeMap * sizeof(float));
	int i = 0;
	while (!keyList.empty()) {
		keyArr[i] = keyList.front();
		data_map[keyArr[i++]] = -1.000;
		keyList.pop();
	}

	/*data_map["/instrumentation/airspeed-indicator/indicated-speed-kt"] = 0.0000;
	data_map["/sim/time/warp"] = 0.0000;
	data_map["/controls/switches/magnetos"] = 0.0000;
	data_map["/instrumentation/heading-indicator/offset-deg"] = 0.0000;
	data_map["/instrumentation/altimeter/indicated-altitude-ft"] = 0.0000;
	data_map["/instrumentation/altimeter/pressure-alt-ft"] = 0.0000;
	data_map["/instrumentation/attitude-indicator/indicated-pitch-deg"] = 0.0000;
	data_map["/instrumentation/attitude-indicator/indicated-roll-deg"] = 0.0000;
	data_map["/instrumentation/attitude-indicator/internal-pitch-deg"] = 0.0000;
	data_map["/instrumentation/attitude-indicator/internal-roll-deg"] = 0.0000;
	data_map["/instrumentation/encoder/indicated-altitude-ft"] = 0.0000;
	data_map["/instrumentation/encoder/pressure-alt-ft"] = 0.0000;
	data_map["/instrumentation/gps/indicated-altitude-ft"] = 0.0000;
	data_map["/instrumentation/gps/indicated-ground-speed-kt"] = 0.0000;
	data_map["/instrumentation/gps/indicated-vertical-speed"] = 0.0000;
	data_map["/instrumentation/heading-indicator/indicated-heading-deg"] = 0.0000;
	data_map["/instrumentation/magnetic-compass/indicated-heading-deg"] = 0.0000;
	data_map["/instrumentation/slip-skid-ball/indicated-slip-skid"] = 0.0000;
	data_map["/instrumentation/turn-indicator/indicated-turn-rate"] = 0.0000;
	data_map["/instrumentation/vertical-speed-indicator/indicated-speed-fpm"] = 0.0000;
	data_map["/controls/flight/aileron"] = 0.0000;
	data_map["/controls/flight/elevator"] = 0.0000;
	data_map["/controls/flight/rudder"] = 0.0000;
	data_map["/controls/flight/flaps"] = 0.0000;
	data_map["/controls/engines/engine/throttle"] = 0.0000;
	data_map["/controls/engines/current-engine/throttle"] = 0.0000;
	data_map["/controls/switches/master-avionics"] = 0.0000;
	data_map["/controls/switches/starter"] = 0.0000;
	data_map["/engines/active-engine/auto-start"] = 0.0000;
	data_map["/controls/flight/speedbrake"] = 0.0000;
	data_map["/sim/model/c172p/brake-parking"] = 0.0000;
	data_map["/controls/engines/engine/primer"] = 0.0000;
	data_map["/controls/engines/current-engine/mixture"] = 0.0000;
	data_map["/controls/switches/master-bat"] = 0.0000;
	data_map["/controls/switches/master-alt"] = 0.0000;
	data_map["/engines/engine/rpm"] = 0.0000;*/
}

Data::~Data()
{
	if (dataArr != NULL)
		delete[] dataArr;
}

bool Data::Update()
{
	for (int i = 0; i < baf_size; i++)
	{
		string str;
		while (BafQueue.front() != ',')
		{
			if (!BafQueue.empty())
			{
				str.push_back(BafQueue.front());
				BafQueue.pop();
			}
			else
			{
				int i = 0;
				while (str.size())
				{
					char c = str[i++];
					BafQueue.push(c);
				}
				baf_size = 1;
				insertToMap();
				return 0;
				
			}
			
		}
		if (!BafQueue.empty())
		{
			BafQueue.pop();
		}
		if (str.size())
		{
			stringstream to_float(str);

			float f = 0;
			to_float >> f;
			if (data_index == sizeMap)
				data_index = 0;
			dataArr[data_index++] = f;
		}
	}
	baf_size = 0;
	insertToMap();
	return 0;
}

bool Data::updateBaf(const char* baf_, int bytesIn)
{
	for (int i = 0; i < bytesIn; i++)
	{
		if (baf_[i] == ',')
			baf_size++;
		if (baf_[i] == '\n')
		{
			baf_size++;
			BafQueue.push(',');
		}
		else
			BafQueue.push(baf_[i]);
	}

	Update();
	return 0;
}

bool Data::insertToMap()
{
	lock_flag_data.lock();

	for (int i = 0; i < sizeMap; i++)
	{
		data_map[keyArr[i]] = dataArr[i];
	}
	lock_flag_data.unlock();

	return 1;
}



float Data::getVelue(string key) 
{
	lock_flag_data.lock();
	float f =  data_map[key];
	lock_flag_data.unlock();
	return f;
}
