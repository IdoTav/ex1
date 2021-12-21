#include <filesystem>
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

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command{
protected:
    DefaultIO* dio;
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
public:
	Command(DefaultIO* dio):dio(dio){}
    Command(DefaultIO* dio, TimeSeries* trainTsC, TimeSeries* testTsC, HybridAnomalyDetector* adC,
            vector<AnomalyReport>* rC):dio(dio), trainTs(trainTsC), testTs(testTsC), ad(adC), r(rC){}
    //Command(int l) {l = 6;}
	virtual void execute()=0;
	virtual ~Command(){}
};


class uploadAtimeSeriesCommand:public Command{
public:
    uploadAtimeSeriesCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                             vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}


    virtual void execute() {
        string w = "Please upload your local test CSV file.\n";
        dio->write(w);
        std::ofstream serverFile("anomalyTrain.csv");
        serverFile.open("anomalyTrain.csv");
        string s = dio->read();
        while (s != "done") {
            serverFile << s + "\n";
            s = dio->read();
        }
        serverFile.close();
        w = "Please upload your local test CSV file.\n";
        dio->write(w);
        std::ofstream serverFile1("anomalyTest.csv");
        serverFile.open("anomalyTest.csv");
        s = dio->read();
        while (s != "done") {
            serverFile1 << s + "\n";
            s = dio->read();
        }
        serverFile1.close();
        TimeSeries tmp = TimeSeries("anomalyTrain.csv");
        this->trainTs = &tmp;
        tmp = TimeSeries("anomalyTest.csv");
        this->testTs = &tmp;
        w = "Upload complete\n";
        dio->write(w);
    }
};


class detectAnomaliesCommand:public Command {
public:
    detectAnomaliesCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                            vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        (*ad).learnNormal(*trainTs);
        *r = (*ad).detect(*testTs);
        string s = "anomaly detection complete.\n";
        dio->write(s);
    }
};

class currentThresholdCommand:public Command{
public:
    currentThresholdCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                            vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        string s = "The current correlation threshold is " + std::to_string(ad->getTopThreshold());
        dio->write(s);
        string threshold = dio->read();
        float newThreshold = std::stof(threshold);
        while (newThreshold < 0 || newThreshold > 1) {
            s = "please choose a value between 0 and 1.\n";
            dio->write(s);
            threshold = dio->read();
            newThreshold = std::stof(threshold);;
        }
        ad->setTopThreshold(newThreshold);
    }
};

class displayCommand:public Command{
public:
    displayCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                   vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute(){
        for (AnomalyReport ar : *r) {
            string s = to_string(ar.timeStep) + " " + ar.description;
            dio->write(s);
        }
    }
};

class analyzeCommand:public Command{
public:
    analyzeCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                   vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute(){
        // insert the client input to vector who holds the data
        vector<pair<int, int>> sumAnomalies;
        string s = dio->read();
        while (s != "done") {
            stringstream rangeSplit(s);
            string num;
            vector<string> rangeNum;
            while (getline(rangeSplit, num, ',')) {
                rangeNum.push_back(num);
            }
            sumAnomalies.emplace_back(stoi(rangeNum[0]), stoi(rangeNum[1]));
            s = dio->read();
        }
        // marge time steps reports
        vector<pair<long, long>> sumReports;
        string lastRep = " ";
        int lastStep = 0;
        for (AnomalyReport ar: *r) {
            if (lastRep == ar.description) {
                if(lastStep + 1 == ar.timeStep) {
                    long first = sumReports.back().first;
                    sumReports.pop_back();
                    sumReports.emplace_back(first, ar.timeStep);
                }
            }
        }
        int TP = 0, FP = 0;
        for (auto it: sumAnomalies) {
            for (AnomalyReport ar: *r) {
                if (ar.timeStep >= it.first) {
                    if (ar.timeStep <= it.second){
                        TP += 1;
                        break;
                    }
                }
            }
        }
    }
};

#endif /* COMMANDS_H_ */
