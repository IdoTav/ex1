//Ido Tavron 316222512
//Tal Dabran 316040898
#include "timeseries.h"
#include "SimpleAnomalyDetector.h"

/**
 * this function get valueVector and insert the values under appropriate key
 * @param valueVector is a values who need to been inserted to the map
 * @param initMap is the map who will contain the data
 */
void TimeSeries::initializeMap(vector<float> valueVector, map<string, vector<float>> *initMap) {
    vector<float> vectorArray[_keysVector.size()];
    for (int i = 0; i < valueVector.size(); i++)
        vectorArray[i % _keysVector.size()].push_back(valueVector[i]);
    int i = 0;
    for (auto it = _keysVector.begin(); it != _keysVector.end(); it++) {
        initMap->insert({_keysVector[i], vectorArray[i]});
        i++;
    }
}

/**
 * this function split string by comma and insert the strings who was seperated to keys vector
 * @param currentLine  string who need to been split by commas
 */
void TimeSeries::getKeysFromString(const string& currentLine) {
    stringstream strArr(currentLine);
    while (strArr.good()) {
        string key;
        getline(strArr, key, ',');
        _keysVector.push_back(key);
    }
}

/**
 * this function split string by comma and insert the strings who was seperated to values vector
 * @param currentLine  string who need to been split by commas
 * @param valueVector vector who will hold the values
 */
void TimeSeries::getValueFromString(const string& currentLine, vector<float> *valueVector) {
    stringstream strArr(currentLine);
    while (strArr.good()) {
        string value;
        getline(strArr, value, ',');
        valueVector->push_back(std::stof(value));
    }
}

/**
 * getter to the keys Vector
 * @return the keys Vector
 */
vector<string> TimeSeries:: getKeysVector()const {
    return _keysVector;
}

/**
* getter for values from the map who has been detected by key
 * @param key help us to detect the values vector in the map
 * @return the specific values who has been detected
 */
vector<float> TimeSeries:: getValuesByKey(const string& key)const {
    auto it = _mapData.find(key);
    return it->second;
}

/**
 * getter for value from the map who has been detected by key and index
 * @param key help us to detect the values vector in the map
 * @param i help us to detect the value in the vector
 * @return the specific value who has been detected
 */
float TimeSeries::getValByKeyAndIndex (const string& key, int i)const {
    vector<float> values = getValuesByKey(key);
    return values[i];
}

/**
 * set the data base and insert the data to a map
 * @param CSVfileName is CSV file name who will be our data
 */
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




