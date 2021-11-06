
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <string>
#include <vector>

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
    for (it = keysVector.begin(); it != keysVector.end(); it++) {
        int arraySize = (int)ts.getValueByKey(*it).size();
        float array1[ts.getValueByKey(*it).size()];
        fromVectorToFloatArray(ts.getValueByKey(*it), array1);
        correlatedFeatures tmp;
        tmp.feature1 = *it;
        float bestCor = 0;
        for(it2 = it + 1; it2 != keysVector.end(); it2++){
            float array2[ts.getValueByKey(*it2).size()];
            fromVectorToFloatArray(ts.getValueByKey(*it2), array2);
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
}

void fromVectorToFloatArray(vector<float>vec, float array[]) {
    for(int i = 0; i<vec.size(); i++) {
        array[i] = vec[i];
    }
}
/*
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
}*/
