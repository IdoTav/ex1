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
    void getKeysFromString(string currentLine);
    void getValueFromString(string currentLine, vector<float>* valueVector);
public:
    vector<float>getValueByKey(const string& key)const;
    vector<string>getKeysVector()const;
    TimeSeries(const char* CSVfileName) ;
};


#endif /* TIMESERIES_H_ */
