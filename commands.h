//Ido Tavron 316222512
//Tal Dabran 316040898
#include <iostream>
#include <fstream>
#include <sys/socket.h>


#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <regex>
#include "HybridAnomalyDetector.h"
#include "AnomalyDetector.h"
#include "CLI.h"

using namespace std;

struct anoRep {
    string description;
    long timeStep;
};

class DefaultIO {
public:

    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual ~DefaultIO() {}

};


class Command {
protected:
    DefaultIO *_dio;
public:
    Command(DefaultIO *dio) : _dio(dio) {}

    bool itIsEndWith (string text, string suff){
        regex re(suff);
        smatch match;
        regex_search(text, match, re);
        return (!match.empty());
    }

    string cutSuffix(string text, string suff) {
        regex re(suff);
        smatch match;
        regex_search(text, match, re);
        return match.prefix();
    }

    string cutPrefix(string text, string pre) {
        regex re(pre);
        smatch match;
        regex_search(text, match, re);
        return match.suffix();

    }

    void MergeSortedIntervals(vector<anoRep> &ar, int s, int m, int e) {
        vector<anoRep> temp;
        int i, j;
        i = s;
        j = m + 1;
        while (i <= m && j <= e) {
            if (ar[i].description <= ar[j].description) {
                temp.push_back(ar[i]);
                ++i;
            } else {
                temp.push_back(ar[j]);
                ++j;
            }
        }
        while (i <= m) {
            temp.push_back(ar[i]);
            ++i;
        }
        while (j <= e) {
            temp.push_back(ar[j]);
            ++j;
        }
        for (int i = s; i <= e; ++i)
            ar[i] = temp[i - s];
    }

    void MergeSort(vector<anoRep> &ar, unsigned long s, unsigned long e) {
        vector<AnomalyReport> sortedVector;
        if (s < e) {
            int m = (s + e) / 2;
            MergeSort(ar, s, m);
            MergeSort(ar, m + 1, e);
            MergeSortedIntervals(ar, s, m, e);
        }
    }

/**
* this function prints only 3 numbers after the decimal point
* @param f, a float variable that we want to print only the 3 numbers after the decimal point
*/
    string cutDecimal(float f) {
        if (1 == f)
            return "1";
        if (0 == f)
            return "0";
        //check how many number there is after the decimal point
        float q = f * 1000;
        int comp = q;
        //check how many number there is after the decimal point
        if (comp < 10)
            return "0.00" + to_string(comp);
        //check how many number there is after the decimal point
        if (comp < 100) {
            if (0 == comp % 10)
                comp = comp / 10;
            return "0.0" + to_string(comp);
        } else {
            if (0 == comp % 100)
                comp = comp / 100;
            return "0." + to_string(comp);
        }
    }

    virtual void execute() = 0;

    virtual ~Command() {}
};

class uploadAtimeSeriesCommand : public Command {
public:
    TimeSeries _trainTs;
    TimeSeries _testTs;
    int _numlines = 0;

    uploadAtimeSeriesCommand(DefaultIO *dio) : Command(dio) {}

    /**
 * this function execute the given command, to upload the train and the test csv
 */
    virtual void execute() {
        _dio->write("Please upload your local train CSV file.\n");
        std::fstream serverFile;
        serverFile.open("anomalyTrain.csv", fstream::app);
        string w = _dio->read();
        //reading until we get done
        //while (w != "done") {
        while (!itIsEndWith(w, "done")) {
            serverFile << w;
            w = _dio->read();
        }
        w = cutSuffix(w, "done");
        serverFile << w;
        serverFile.close();
        _dio->write("Upload complete.\n");
        this->_trainTs = TimeSeries("anomalyTrain.csv");
        _dio->write("Please upload your local test CSV file.\n");
        std::ofstream serverFile1;
        serverFile1.open("anomalyTest.csv", fstream::app);
        w = _dio->read();
        //reading from the file until the done
        while (!itIsEndWith(w, "done")) {
            serverFile1 << w;
            w = _dio->read();
        }
        w = cutSuffix(w, "done");
        serverFile1 << w;
        serverFile1.close();
        _dio->write("Upload complete.\n");
        this->_testTs = TimeSeries("anomalyTest.csv");
        vector<string> keys = _testTs.getKeysVector();
        _numlines = _testTs.getValuesByKey(keys[0]).size();
    }
};

class currentThresholdCommand : public Command {
public:
    HybridAnomalyDetector _ad;

    currentThresholdCommand(DefaultIO *dio, HybridAnomalyDetector ad) : Command(dio) {
        _ad.setTopThreshold(ad.getTopThreshold());
    }

    /**
* this function execute the given command, printing and chaning the threshold
*/
    virtual void execute() {
        //prints the current threshold
        _dio->write("The current correlation threshold is " + cutDecimal(_ad.getTopThreshold()) + "\n");
        _dio->write("Type a new threshold\n");
        string threshold = _dio->read();
        float newThreshold = std::stof(threshold);
        //check if the threshold is valid
        while (newThreshold <= 0 || newThreshold >= 1) {
            _dio->write("please choose a value between 0 and 1.\n");
            _dio->write("The current correlation threshold is " + cutDecimal(_ad.getTopThreshold()) + "\n");
            _dio->write("Type a new threshold\n");
            threshold = _dio->read();
            newThreshold = std::stof(threshold);
        }
        //set the new threshold
        _ad.setTopThreshold(newThreshold);
    }
};

class detectAnomaliesCommand : public Command {
public:
    HybridAnomalyDetector *_ad;
    vector<AnomalyReport> _ar;
    TimeSeries _trainTs;
    TimeSeries _testTs;

    detectAnomaliesCommand(DefaultIO *dio, HybridAnomalyDetector *ad, TimeSeries trainTs, TimeSeries testTs) :
            Command(dio) {
        _ad = ad;
        _trainTs = trainTs;
        _testTs = testTs;
    }

    /**
    * this function execute the given command, to detect the anomalies
    */
    virtual void execute() {
        //calls t learnNormal method
        _ad->learnNormal(_trainTs);
        //calls the detect method
        vector<AnomalyReport> r = _ad->detect(_testTs);
        unsigned long vectorSize = r.size();
        if (!_ar.empty())
            _ar.clear();
        for (int i = 0; i < vectorSize; i++)
            _ar.push_back(r[i]);
        _dio->write("anomaly detection complete.\n");
    }
};

class displayResult : public Command {
public:
    vector<AnomalyReport> *_ar;

    displayResult(DefaultIO *dio, vector<AnomalyReport> *ar) : Command(dio) {
        _ar = ar;
    }

    /**
* this function execute the given command, to display the results
*/
    virtual void execute() {
        vector<anoRep> sortedVector;
        for (AnomalyReport anoR: *_ar) {
            anoRep aR;
            aR.description = anoR.description;
            aR.timeStep = anoR.timeStep;
            sortedVector.push_back(aR);
        }
        MergeSort(sortedVector, 0, sortedVector.size() - 1);
        //goes all over the anomaly reports and prints the time step and the description
        for (anoRep anoR: sortedVector)
            _dio->write(to_string(anoR.timeStep) + "\t" + anoR.description + "\n");
        _dio->write("Done.\n");
    }
};

class analyzeresults : public Command {
public:
    vector<AnomalyReport> *_ar;
    int _numLine;

    analyzeresults(DefaultIO *dio, vector<AnomalyReport> *ar, int numLine) : Command(dio) {
        _ar = ar;
        _numLine = numLine;
    }


    /**
* this function execute the given command, to upload the anomalies and analyze the results
*/
    virtual void execute() {
        // insert the client input to vector who holds the data
        _dio->write("Please upload your local anomalies file.\n");
        vector<pair<int, int>> sumAnomalies;
        string w = _dio->read();
        if (w == "\n")
            string w = _dio->read();
        int P = 0;
        while (cutSuffix(w, "done\n") != "") {
            P += 1;
            string numbers = cutSuffix(w, "\n");
            w = cutPrefix(w, "\n");
            stringstream rangeSplit(numbers);
            string num;
            vector<string> rangeNum;
            while (getline(rangeSplit, num, ',')) {
                rangeNum.push_back(num);
            }
            sumAnomalies.emplace_back(stoi(rangeNum[0]), stoi(rangeNum[1]));
        }
        _dio->write("Upload complete.\n");
        // marge time steps reports
        vector<pair<long, long>> sumReports;
        string lastRep = " ";
        long lastStep = 0;
        for (AnomalyReport r: *_ar) {
            // case the reports are close
            if (lastRep == r.description) {
                if (lastStep + 1 == r.timeStep) {
                    long first = sumReports.back().first;
                    //pop the last range and push new range
                    sumReports.pop_back();
                    sumReports.emplace_back(first, r.timeStep);
                } else
                    sumReports.emplace_back(r.timeStep, r.timeStep);
            }
                // case new description report
            else
                sumReports.emplace_back(r.timeStep, r.timeStep);
            lastStep = r.timeStep;
            lastRep = r.description;
        }
        // find false positive reports.
        float TP = 0, FP = 0;
        for (auto rep: sumReports) {
            int flag = 0;
            for (auto ano: sumAnomalies) {
                // case the report had shared time with the anomaly
                if (rep.first <= ano.first && rep.second >= ano.first)
                    flag = 1;
                else if (rep.first >= ano.first && rep.first <= ano.second)
                    flag = 1;
            }
            // case we didn't break so the report is false
            if (0 == flag)
                FP += 1;
        }
        // find true positive reports.
        int countAno = 0;
        for (auto ano: sumAnomalies) {
            int flag = 0;
            for (auto rep: sumReports) {
                // case the report had shared time with the anomaly
                if (rep.first <= ano.first && rep.second >= ano.first) {
                    TP += 1;
                    break;
                } else if (rep.first >= ano.first && rep.first <= ano.second) {
                    TP += 1;
                    break;
                }
            }
            countAno += ano.second- ano.first + 1;
        }
        int N = _numLine - countAno;
        // write true positive rate
        float tRate = (TP / P);
        _dio->write("True Positive Rate: " + cutDecimal(tRate) + "\n");
        // write false positive rate
        float nRate = (FP / N);
        _dio->write("False Positive Rate: " + cutDecimal(nRate) + "\n");
    }
};

#endif /* COMMANDS_H_ */
