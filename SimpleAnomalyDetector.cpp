
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <string>
#include <vector>
#define THERSHOLD 0.5

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void initPointsArray (float* feature1, float* feature2, int size, Point** array, Point p[]) {
    for (int i =0; i < size; i++) {
        array[i] = &p[i];
    }
    int b =0;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string>keysVector = ts.getKeysVector();
    vector<string>::iterator it;
    vector<string>::iterator it2;
    for (it = keysVector.begin(); it != keysVector.end(); it++) {
        int arraySize = (int)ts.getValueByKey(*it).size();
        float array1[ts.getValueByKey(*it).size()];
        fromVectorToFloatArray(ts.getValueByKey(*it), array1);
        correlatedFeatures tmp;
        /*check*/
        tmp.feature1 = *it;
        float bestCor = THERSHOLD;
        Point* pointArr[arraySize];
        for(it2 = it + 1; it2 != keysVector.end(); it2++){
            float array2[ts.getValueByKey(*it2).size()];
            fromVectorToFloatArray(ts.getValueByKey(*it2), array2);
            float curPearson = absolute(pearson(array1, array2, arraySize));
            if (curPearson >= bestCor) {
                tmp.feature2 = *it2;
                tmp.corrlation = curPearson;
                Point p[arraySize];
                for (int i = 0; i < arraySize; i++) {
                    p[i] = Point(array2[i],array1[i]);
                }
                initPointsArray(array2, array1, arraySize, pointArr, p);
                tmp.lin_reg = linear_reg(pointArr, arraySize);
                bestCor = curPearson;
            }
        }
        float maxDev = getMaxDev(pointArr, tmp.lin_reg, arraySize);
        if(-1 == maxDev)
            continue;
        else
            tmp.threshold = maxDev;
        cf.push_back(tmp);
    }
    int a =0;
}

float getMaxDev(Point** pointArr, Line line, int size) {
    try {
        float maxDev = 0;
        for (int i = 0; i < size; i++) {
            float tmpDev = dev(*pointArr[i], line);
            if (tmpDev >= maxDev)
                maxDev = tmpDev;
        }
        return maxDev;
    }
        catch(...) {
            return -1;
        }
    }

void fromVectorToFloatArray(vector<float>vec, float array[]) {
    for(int i = 0; i<vec.size(); i++) {
        array[i] = vec[i];
    }
}
/*
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
}*/
