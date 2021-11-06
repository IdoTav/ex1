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

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};


class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
};

void fromVectorToFloatArray(vector<float>, float array[]);
float getMaxDev(Point** ptrPointArr, Line line, int size);


#endif /* SIMPLEANOMALYDETECTOR_H_ */
