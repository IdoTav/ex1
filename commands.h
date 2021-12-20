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
        std::cout << "Please upload your local test CSV file." << std::endl;
        std::ofstream serverFile("anomalyTrain.csv");
        std::ifstream clientFile;
        clientFile.open("trainFile.csv", ios::in);
        serverFile.open("anomalyTrain.csv", ios::out);
        string tp;
        while (getline(clientFile, tp)) {
            if (tp == "done")
                break;
            serverFile << tp;
        }
        serverFile.close();
        clientFile.close();
        TimeSeries tmp = TimeSeries("anomalyTrain.csv");
        trainTs = &tmp;
        std::cout << "Please upload your local train CSV file." << std::endl;
        std::ofstream serverFile1("anomalyTest.csv");
        std::ifstream clientFile1;
        clientFile1.open("testFile.csv", ios::in);
        serverFile1.open("anomalyTest.csv", ios::out);
        while (getline(clientFile1, tp)) {
            if (tp == "done")
                break;
            serverFile1 << tp;
        }
        std::cout << "Upload complete" << std::endl;
        serverFile1.close();
        clientFile1.close();
        TimeSeries tmp2 = TimeSeries("anomalyTest.csv");
        testTs = &tmp2;
    }
};


class detectAnomaliesCommand:public Command {
public:
    detectAnomaliesCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                            vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        (*ad).learnNormal(*trainTs);
        *r = (*ad).detect(*testTs);
        std::cout << "anomaly detection complete." << std::endl;
    }
};

class currentThresholdCommand:public Command{
public:
    currentThresholdCommand(DefaultIO* dio, TimeSeries *trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                            vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        std::cout << "The current correlation threshold is" << " " << ad->getTopThreshold() << std::endl;
        float newThreshold;
        std::cin >> newThreshold;
        while (newThreshold < 0 || newThreshold > 1) {
            std::cout <<"please choose a value between 0 and 1." << std::endl;
            std::cin >> newThreshold;
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
            std::cout << to_string(ar.timeStep) << " " << ar.description << std::endl;
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
