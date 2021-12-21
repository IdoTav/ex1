#include <iostream>
#include <fstream>
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include "AnomalyDetector.h"
#include "CLI.h"

using namespace std;
class DefaultIO{
public:


    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    virtual ~DefaultIO(){}

};

class Command{
protected:
    DefaultIO* _dio;
public:
    Command(DefaultIO* dio):_dio(dio){}
    virtual void execute()=0;
    virtual ~Command(){}
};


class uploadAtimeSeriesCommand:public Command{
public:
    TimeSeries _trainTs;
    TimeSeries _testTs;
    int _numlines = -1;
    uploadAtimeSeriesCommand(DefaultIO* dio) : Command(dio){}
    virtual void execute() {
        string s = "Please upload your local train CSV file.\n";
        _dio->write(s);
        std::fstream serverFile;
        serverFile.open("anomalyTrain.csv", fstream ::app);
        string w = _dio->read();
        while (w != "done") {
            serverFile << w + "\n";
            w = _dio->read();
            _numlines += 1;
        }
        serverFile.close();
        this->_trainTs = TimeSeries("anomalyTrain.csv");
        s = "Please upload your local test CSV file.\n";
        _dio->write(s);
        std::ofstream serverFile1;
        serverFile1.open("anomalyTest.csv", fstream ::app);
        w = _dio->read();
        while (w != "done") {
            serverFile1 << w + "\n";
            w = _dio->read();
        }
        serverFile1.close();
        s = "Upload complete\n";
        _dio->write(s);
        this->_testTs = TimeSeries("anomalyTest.csv");
    }
};


class currentThresholdCommand:public Command{
public:
    HybridAnomalyDetector _ad;
    currentThresholdCommand(DefaultIO* dio) : Command(dio) {}
    virtual void execute(){
        string s = "The current correlation threshold is " + to_string(_ad.getTopThreshold()) + "\n";
        _dio->write(s);
        s = "Type a new threshold\n";
        _dio->write(s);
        string threshold = _dio->read();
        float newThreshold = std::stof(threshold);
        while (newThreshold < 0 || newThreshold > 1) {
            s = "please choose a value between 0 and 1.\n";
            _dio->write(s);
            threshold = _dio->read();
            newThreshold = std::stof(threshold);
        }
        _ad.setTopThreshold(newThreshold);
    }
};

class detectAnomaliesCommand:public Command {
public:
    HybridAnomalyDetector* _ad;
    vector <AnomalyReport> _ar;
    TimeSeries _trainTs;
    TimeSeries _testTs;
    detectAnomaliesCommand(DefaultIO *dio, HybridAnomalyDetector* ad, TimeSeries trainTs, TimeSeries testTs) :
    Command(dio) {
            _ad = ad;
            _trainTs = trainTs;
            _testTs = testTs;
    }
    virtual void execute(){
        _ad->learnNormal(_trainTs);
        vector<AnomalyReport> r = _ad->detect(_testTs);
        unsigned long vectorSize = r.size();
        for (int i = 0; i < vectorSize ; i ++) {
            _ar.push_back(r[i]);
        }
        string s = "anomaly detection complete.\n";
        _dio->write(s);
    }
};
/**
class displayCommand:public Command{
    vector<AnomalyReport> _r;
public:
    displayCommand(DefaultIO *dio1, DefaultIO *dio, vector<AnomalyReport> r) : Command(dio1) {
        _r = r;
    }
    virtual void execute(){
        ad->learnNormal(*trainTs);
        _r = ad->detect(*testTs);
        string s = "anomaly detection complete.\n";
        _dio->write(s);
    }
};

class analyzeCommand:public Command{
public:
    analyzeCommand(DefaultIO* dio) : Command(dio) {}
    virtual void execute(){

    }
};

*/
#endif /* COMMANDS_H_ */
