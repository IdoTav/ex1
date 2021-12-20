//Ido Tavron 316222512
//Tal Dabran 316040898
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_
#include "timeseries.h"
#include "anomaly_detection_util.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "fstream"
#include "AnomalyDetector.h"
#include "minCircle.h"

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Circle min_cir;
    float threshold;
};


class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector{
public:
    float topThreshold = 0.9;
    float botThreshold = 0.5;
    vector<correlatedFeatures> cf;
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void pickMethod(correlatedFeatures* cor, float curPearson, float upThresh, float lowThresh,
                          int arraySize, string it2, Point** pointArr, Point* p,
                          float* array1, float* array2);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    float getTopThreshold(){
        return this->topThreshold;
    }
    float getBotThreshold(){
        return this->botThreshold;
    }
    void setTopThreshold(float num){
        this->topThreshold = num;
    }
};

void fromVectorToFloatArray(vector<float>, float array[]);
float getMaxDev(Point** ptrPointArr, Line line, int size);
void initPointsArray (int size, Point** ptrArray, Point p[]);
AnomalyReport addReport(correlatedFeatures* cor, int i);


#endif /* SIMPLEANOMALYDETECTOR_H_ */