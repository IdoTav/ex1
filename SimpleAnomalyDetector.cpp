
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

void initPointsArray (float* feature1, float* feature2, int size, Point** array) {
    for (int i = 0; i < size; i++) {
        Point p(feature1[i], feature2[i]);
        array[i] = &p;
    }
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string>keysVector = ts.getKeysVector();
    vector<string>::iterator it;
    vector<string>::iterator it2;
    int i= 0;
    int arraySize = ts.getValueByKey(*it).size();
    for (it = keysVector.begin(); it != keysVector.end(); it++) {
        float* array1 = fromVectorToFloatArray(ts.getValueByKey(*it));
        correlatedFeatures tmp;
        tmp.feature1 = *it;
        float bestCor = 0;
        for( it2 = it + 1; it2 != keysVector.end(); it2++){
            float* array2 = fromVectorToFloatArray(ts.getValueByKey(*it));
            float curPearson = absolute(pearson(array1, array2, arraySize));
            if (curPearson >= bestCor) {
                tmp.feature2 = *it2;
                tmp.corrlation = curPearson;
                Point* pointArr[arraySize];
                initPointsArray(array2, array1, arraySize, pointArr);
                tmp.lin_reg = linear_reg(pointArr, arraySize);
                bestCor = curPearson;
            }
        }
        cf.push_back(tmp);
    }
    int a = 10;
}

float* fromVectorToFloatArray(vector<float>vec) {
    float floatArray[vec.size()];
    for(int i = 0; i<vec.size(); i++) {
        floatArray[i] = vec[i];
    }
}
/*
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
}*/
