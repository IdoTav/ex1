
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <string>
#include <vector>
#define THERSHOLD 0.5


float getMaxDev(Point** pointArr, Line line, int size) {
    if (0 == line.a && 0 == line.b)
        return -1;
    else {
        float maxDev = 0;
        for (int i = 0; i < size; i++) {
            float tmpDev = dev(*pointArr[i], line);
            if (tmpDev >= maxDev)
                maxDev = tmpDev;
        }
        return maxDev;
    }
}

void fromVectorToFloatArray(vector<float>vec, float array[]) {
    for(int i = 0; i<vec.size(); i++) {
        array[i] = vec[i];
    }
}

void initPointsArray (int size, Point** array, Point p[]) {
    for (int i =0; i < size; i++) {
        array[i] = &p[i];
    }
}

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
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
        float bestCor = THERSHOLD;
        Point* pointArr[arraySize];
        Point p[arraySize];
        for(it2 = it + 1; it2 != keysVector.end(); it2++){
            float array2[ts.getValueByKey(*it2).size()];
            fromVectorToFloatArray(ts.getValueByKey(*it2), array2);
            float curPearson = absolute(pearson(array1, array2, arraySize));
            if (curPearson >= bestCor) {
                tmp.feature2 = *it2;
                tmp.corrlation = curPearson;
                for (int i = 0; i < arraySize; i++) {
                    p[i] = Point(array1[i],array2[i]);
                }
                initPointsArray(arraySize, pointArr, p);
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
}


vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<correlatedFeatures>::iterator it;
    vector<AnomalyReport> arVector;
    for(int i = 0; i < ts.getValueByKey(ts.getKeysVector()[1]).size(); i++) {
        int j = 0;
        for (it = cf.begin(); it != cf.end(); it++) {
            float xVal = ts.getValByKeyAndIndex(cf[j].feature1, i), yVal = ts.getValByKeyAndIndex(cf[j].feature2, i);
            float curDev = cf[j].threshold;
            Point p(xVal, yVal);
            if (curDev < dev(p, cf[j].lin_reg)) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                AnomalyReport ar(desc, i + 1);
                arVector.push_back(ar);
            }
            j++;
        }
    }
    return arVector;
}
