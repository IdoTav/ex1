
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include "timeseries.h"
#include <string>
#include <vector>
#define THERSHOLD 0.9

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/**
 * this function get ptrPointArr and line, she is returns the most long distance between the
 * given line to the farest point
 * @param ptrPointArr is a pointer to array with pointers who points to Point objects
 * @param line is a given line
 * @param size is the pointers array size
 * @return the farest point from the line
 */
float getMaxDev(Point** ptrPointArr, Line line, int size) {
    //case the line was created by default constructor, so we can do nothing with him.
    if (0 == line.a && 0 == line.b)
        return -1;
    else {
        //initialize variable who hold the maximum value
        float maxDev = 0;
        //run all over the Point objects from the pointers array
        for (int i = 0; i < size; i++) {
            //call the function who calculate the distance between given point to the line
            float tmpDev = dev(*ptrPointArr[i], line);
            if (tmpDev >= maxDev)
                maxDev = tmpDev;
        }
        return maxDev;
    }
}


/**
 * this function takes floats vector and convert him to the array
 * @param vec is vector who will be copied to an array
 * @param array is a pointer to float array who will contain the floats from the vector
 */
void fromVectorToFloatArray(vector<float>vec, float array[]) {
    for(int i = 0; i<vec.size(); i++) {
        array[i] = vec[i];
    }
}


/**
 * this function takes pointer to ptrArray with pointers who points to Point objects and inserts the Point objects
 * to simple Points ptrArray
 * @param size is the pointers array size
 * @param ptrArray pointer to array with pointers who points to Point objects
 * @param p is a pointer to Point objects array who will contain the points from ptrArray
 */
void initPointsArray (int size, Point** ptrArray, Point p[]) {
    for (int i =0; i < size; i++) {
        ptrArray[i] = &p[i];
    }
}

/**
 * this function gets data and run a normal learn model to find the best correlations between any features in given
 * time series
 * @param ts hold the data from given database in map
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    vector<string>keysVector = ts.getKeysVector();
    //run the main keys (feature 1)
    for (auto it = keysVector.begin(); it != keysVector.end(); it++) {
        int arraySize = (int) ts.getValuesByKey(*it).size();
        float array1[ts.getValuesByKey(*it).size()];
        fromVectorToFloatArray(ts.getValuesByKey(*it), array1);
        correlatedFeatures tmp;
        tmp.feature1 = *it;
        //set the minimum threshold we can live with
        float bestCor = THERSHOLD;
        Point* pointArr[arraySize];
        Point p[arraySize];
        // run the second key (feature 2) by initializing the iterator to the next to the main key
        for(auto it2 = it + 1; it2 != keysVector.end(); it2++){
            float array2[ts.getValuesByKey(*it2).size()];
            fromVectorToFloatArray(ts.getValuesByKey(*it2), array2);
            float curPearson = absolute(pearson(array1, array2, arraySize));
            //if we find a batter correlation between the main key to another key set him into the correlated feature
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
        //case there is no maxDev
        if(-1 == maxDev)
            continue;
        //make sure the threshold will be accurate
        else
            tmp.threshold = (float) 1.1 * maxDev;
        //add the correlated feature to the correlated features vector
        cf.push_back(tmp);
    }
}

/**
 * this function gets data and looking for anomalies between the data to the normal model who has been learned
 * before
 * @param ts hold the data from given database in map
 * @return vector with anomaly reports
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> arVector;
    //run all over the rows on the given data
    for(int i = 0; i < ts.getValuesByKey(ts.getKeysVector()[1]).size(); i++) {
        int j = 0;
        //looking for correlated features who can be compared with specific points in a row
        for (auto it = cf.begin(); it != cf.end(); it++) {
            float xVal = ts.getValByKeyAndIndex(cf[j].feature1, i), yVal = ts.getValByKeyAndIndex(cf[j].feature2, i);
            float curDev = cf[j].threshold;
            Point p(xVal, yVal);
            //check if there is an anomaly
            if (curDev < dev(p, cf[j].lin_reg)) {
                string desc = cf[j].feature1 + "-" + cf[j].feature2;
                AnomalyReport ar(desc, i + 1);
                //add the anomaly to the anomaly reports
                arVector.push_back(ar);
            }
            j++;
        }
    }
    return arVector;
}
