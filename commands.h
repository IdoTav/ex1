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
    uploadAtimeSeriesCommand(DefaultIO* dio, TimeSeries* trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                             vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        string s = "Please upload your local train CSV file.\n";
        dio->write(s);
        std::fstream serverFile;
        serverFile.open("anomalyTrain.csv", fstream ::app);
        string w = dio->read();
        while (w != "done") {
            serverFile << w + "\n";
            w = dio->read();
        }
        serverFile.close();
        TimeSeries tmp = TimeSeries("anomalyTrain.csv");
        *trainTs = tmp;
        s = "Please upload your local test CSV file.\n";
        dio->write(s);
        std::ofstream serverFile1;
        serverFile1.open("anomalyTest.csv", fstream ::app);
        w = dio->read();
        while (w != "done") {
            serverFile1 << w + "\n";
            w = dio->read();
        }
        serverFile1.close();
        s = "Upload complete\n";
        dio->write(s);
        TimeSeries tmp2 = TimeSeries("anomalyTest.csv");
        *testTs = tmp2;
    }
};


class detectAnomaliesCommand:public Command {
public:
    detectAnomaliesCommand(DefaultIO* dio, TimeSeries* trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                           vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        (*ad).learnNormal(*trainTs);
        *r = (*ad).detect(*testTs);
        std::cout << "anomaly detection complete." << std::endl;
    }
};

class currentThresholdCommand:public Command{
public:
    currentThresholdCommand(DefaultIO* dio, TimeSeries* trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                            vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute() {
        string s = "The current correlation threshold is " + to_string(ad->getTopThreshold()) + "\n";
        dio->write(s);
        string threshold = dio->read();
        float newThreshold = std::stof(threshold);
        while (newThreshold < 0 || newThreshold > 1) {
            s = "please choose a value between 0 and 1.\n";
            dio->write(s);
            threshold = dio->read();
            newThreshold = std::stof(threshold);
        }
        ad->setTopThreshold(newThreshold);
    }
};

class displayCommand:public Command{
public:
    displayCommand(DefaultIO* dio, TimeSeries* trainTsC, TimeSeries *testTsC, HybridAnomalyDetector* adC,
                   vector<AnomalyReport> *rC) : Command(dio, trainTsC, testTsC, adC, rC) {}
    virtual void execute(){
        for (AnomalyReport ar : *r) {
            string s = to_string(ar.timeStep) + " " + ar.description + "\n";
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
        int P = 0;
        while (s != "done") {
            P += 1;
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
        long lastStep = 0;
        for (AnomalyReport ar: *r) {
            // case the reports are close
            if (lastRep == ar.description) {
                if(lastStep + 1 == ar.timeStep) {
                    long first = sumReports.back().first;
                    //pop the last range and push new range
                    sumReports.pop_back();
                    sumReports.emplace_back(first, ar.timeStep);
                } else {
                    sumReports.emplace_back(ar.timeStep, ar.timeStep);
                }
            }
                // case new description report
            else {
                sumReports.emplace_back(ar.timeStep, ar.timeStep);
            }
            lastStep = ar.timeStep;
            lastRep = ar.description;
        }
        // find false positive reports.
        float TP = 0, FP = 0;
        for (auto rep: sumReports) {
            int flag = 0;
            for (auto ano: sumAnomalies) {
                // case the report had shared time with the anomaly
                if (rep.first <= ano.first && rep.second >= ano.first){
                    break;
                } else if (rep.first >= ano.first && rep.first <= ano.second){
                    break;
                }
            }
            // case we didn't break so the report is false
            FP += 1;
        }
        // find true positive reports.
        for (auto ano: sumAnomalies) {
            int flag = 0;
            for (auto rep: sumReports) {
                // case the report had shared time with the anomaly
                if (rep.first <= ano.first && rep.second >= ano.first){
                    TP += 1;
                    break;
                } else if (rep.first >= ano.first && rep.first <= ano.second){
                    TP += 1;
                    break;
                }
            }
        }
        /* TODO CHANGE N*/
        int N = 4;
        // write true positive rate
        float tRate =(TP/P);
        int tmpRate = tRate * 1000;
        tRate = tmpRate / 1000;
        dio->write("True Positive Rate: ");
        dio->write(tRate);
        dio->write("\n");
        // write false positive rate
        float nRate =(FP/(N));
        tmpRate = nRate * 1000;
        nRate = tmpRate / 1000;
        dio->write("False Positive Rate: ");
        dio->write(nRate);
        dio->write("\n");
    }
};

#endif /* COMMANDS_H_ */
