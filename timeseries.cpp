#include "timeseries.h"
#include "SimpleAnomalyDetector.h"

void TimeSeries::initializeMap(vector<float> valueVector, map<string, vector<float>> *initMap) {
    vector<float> vectorArray[_keysVector.size()];
    for (int i = 0; i < valueVector.size(); i++)
        vectorArray[i % _keysVector.size()].push_back(valueVector[i]);
    vector<string>::iterator it;
    int i = 0;
    for (it = _keysVector.begin(); it != _keysVector.end(); it++) {
        initMap->insert({_keysVector[i], vectorArray[i]});
        i++;
    }
}

void TimeSeries::getKeysFromString(const string& currentLine) {
    stringstream strArr(currentLine);
    while (strArr.good()) {
        string key;
        getline(strArr, key, ',');
        _keysVector.push_back(key);
    }
}

void TimeSeries::getValueFromString(const string& currentLine, vector<float> *valueVector) {
    stringstream strArr(currentLine);
    while (strArr.good()) {
        string value;
        getline(strArr, value, ',');
        valueVector->push_back(std::stof(value));
    }
}

vector<string> TimeSeries:: getKeysVector()const {
    return _keysVector;
}

vector<float> TimeSeries:: getValueByKey(const string& key)const {
    auto it = _mapData.find(key);
    return it->second;
}

TimeSeries::TimeSeries(const char *CSVfileName) {
    ifstream myFile;
    myFile.open(CSVfileName);
    string currentLine;
    int numLine = 0;
    vector<float> valueVector;
    while (getline(myFile, currentLine)) {
        if (0 == numLine)
            getKeysFromString(currentLine);
        else
            getValueFromString(currentLine, &valueVector);
        numLine++;
    }
    initializeMap(valueVector, &_mapData);
}


int main() {
    TimeSeries a("123.csv");
    SimpleAnomalyDetector s;
    s.learnNormal(a);
    return 0;
}



