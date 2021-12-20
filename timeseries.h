//Ido Tavron 316222512
//Tal Dabran 316040898
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "timeseries.h"
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "fstream"
using namespace std;
class TimeSeries {
    map<string, vector<float>>_mapData;
    vector<string>_keysVector;
    void initializeMap(vector<float> valueVector,map<string,vector<float>>* initMap);
    void getKeysFromString(const string& currentLine);
    void getValueFromString(const string& currentLine, vector<float>* valueVector);
public:
    TimeSeries(){};
    TimeSeries(const char* CSVfileName);
    vector<float>getValuesByKey(const string& key)const;
    vector<string>getKeysVector()const;
    float getValByKeyAndIndex (const string& key, int i)const;
};


#endif /* TIMESERIES_H_ */
