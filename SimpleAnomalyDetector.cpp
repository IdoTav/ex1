
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "fstream"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string>keysVector = ts.getKeysVector();
    vector<string>::iterator it;
    vector<string>::iterator it2;
    int i= 0;
    for (it = keysVector.begin(); it != keysVector.end(); it++) {
        float* array1 = fromVectorToFloatArray(ts.getValueByKey(*it));
        for( it2 = it + 1; it2 != keysVector.end(); it2++){
            float* array2 = fromVectorToFloatArray(ts.getValueByKey(*it));
        }
    }
}

float* fromVectorToFloatArray(vector<float>vec) {
    float floatArray[vec.size()];
    for(int i = 0; i<vec.size(); i++) {
        floatArray[i] = vec[i];
    }
}
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
}

