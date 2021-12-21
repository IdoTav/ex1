#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}


void printMenu(DefaultIO* dio) {
    string menu = "Welcome to the Anomaly Detection Server.\n"
                  "Please choose an option:\n"
                  "1.upload a time series csv file\n"
                  "2.algorithm settings\n"
                  "3.detect anomalies\n"
                  "4.display results\n"
                  "5.upload anomalies and analyze results\n"
                  "6.exit\n";
    dio->write(menu);
}

string cutDecimal (float f) {
    if (1 == f)
        return "1";
    if (0 == f)
        return "0";
    float q = f * 1000;
    int comp = q;
    if (comp < 10)
        return "0.00"+ to_string(comp);
    if (comp < 100) {
        if (0 == comp % 10)
            comp = comp / 10;
        return "0.0" + to_string(comp);
    }
    else {
        if (0 == comp % 100)
            comp = comp / 100;
        return "0." + to_string(comp);
    }
}
/**
void CLI::setTrainTs(DefaultIO *dio) {
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
    this->trainTs = &tmp;
}

void CLI::setTestTs (DefaultIO* dio) {
    string s = "Please upload your local test CSV file.\n";
    dio->write(s);
    std::ofstream serverFile1;
    serverFile1.open("anomalyTest.csv", fstream ::app);
    string w = dio->read();
    while (w != "done") {
        serverFile1 << w + "\n";
        w = dio->read();
    }
    serverFile1.close();
    s = "Upload complete\n";
    dio->write(s);
    TimeSeries tmp = TimeSeries("anomalyTest.csv");
    this->testTs = &tmp;
}

void CLI::setSettings (DefaultIO* dio) {
    string s = "The current correlation threshold is " + to_string(this->ad.getTopThreshold()) + "\n";
    dio->write(s);
    s = "Type a new threshold\n";
    dio->write(s);
    string threshold = dio->read();
    float newThreshold = std::stof(threshold);
    while (newThreshold < 0 || newThreshold > 1) {
        s = "please choose a value between 0 and 1.\n";
        dio->write(s);
        threshold = dio->read();
        newThreshold = std::stof(threshold);
    }
    this->ad.setTopThreshold(newThreshold);
}

void CLI::runDetect (DefaultIO* dio) {
    this->ad.learnNormal(*trainTs);
    vector <AnomalyReport> ar = ad.detect(*testTs);
    this->r = &ar;
    string s = "anomaly detection complete.\n";
    dio->write(s);
}

void CLI::runDisplay (DefaultIO* dio) {
    for (AnomalyReport anoR : *(this->r)) {
        string s = to_string(anoR.timeStep) + "\t" + anoR.description + "\n";
        dio->write(s);
    }
    string s = "Done.\n";
    dio->write(s);
}

void CLI:: runAnalyze (DefaultIO* dio) {
    // insert the client input to vector who holds the data
    string s = "Please upload your local anomalies file.\n";
    dio->write(s);
    vector<pair<int, int>> sumAnomalies;
    string w = dio->read();
    int P = 0;
    while (w != "done") {
        P += 1;
        stringstream rangeSplit(w);
        string num;
        vector<string> rangeNum;
        while (getline(rangeSplit, num, ',')) {
            rangeNum.push_back(num);
        }
        sumAnomalies.emplace_back(stoi(rangeNum[0]), stoi(rangeNum[1]));
        w = dio->read();
    }
    s = "Upload complete.\n";
    dio->write(s);
    // marge time steps reports
    vector<pair<long, long>> sumReports;
    string lastRep = " ";
    long lastStep = 0;
    for (AnomalyReport ar: *(this->r)) {
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
                flag = 1;
            } else if (rep.first >= ano.first && rep.first <= ano.second){
                flag = 1;
            }
        }
        // case we didn't break so the report is false
        if (0 == flag)
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
    //TODO CHANGE N
    int N = 1;
    // write true positive rate
    float tRate =(TP/P);
    dio->write("True Positive Rate: " +  cutDecimal(tRate) + "\n");
    // write false positive rate
    float nRate =(FP/(N));
    dio->write("False Positive Rate: " + cutDecimal(nRate) + "\n");
}
**/

void CLI::start() {
    TimeSeries trainTs, testTs;
    HybridAnomalyDetector ad;
    vector<AnomalyReport> ar;
    int numlines;
    while (true) {
        printMenu(this->dio);
        string index = this->dio->read();
        int i = stoi(index);
        if (6 == i)
            break;
        switch (i) {
            case (1): {
                uploadAtimeSeriesCommand ex(dio);
                ex.execute();
                testTs = ex._testTs;
                trainTs = ex._trainTs;
                numlines = ex._numlines;
                break;
            }
            case (2): {
                currentThresholdCommand ex(dio);
                ex.execute();
                ad = ex._ad;
                break;
            }

            case(3): {
                detectAnomaliesCommand ex(dio, &ad, trainTs, testTs);
                ex.execute();
                vector<AnomalyReport> r = ex._ar;
                unsigned long vectorSize = r.size();
                for (int i = 0; i < vectorSize ; i ++) {
                    ar.push_back(r[i]);
                }
                break;
            }
                /**
                case(4): {
                    for (AnomalyReport anoR : *ar) {
                        string s = to_string(anoR.timeStep) + "\t" + anoR.description + "\n";
                        dio->write(s);
                    }
                    string s = "Done.\n";
                    dio->write(s);
                    break;
                }

                case(5): {
                    // insert the client input to vector who holds the data
                    string s = "Please upload your local anomalies file.\n";
                    dio->write(s);
                    vector<pair<int, int>> sumAnomalies;
                    string w = dio->read();
                    int P = 0;
                    while (w != "done") {
                        P += 1;
                        stringstream rangeSplit(w);
                        string num;
                        vector<string> rangeNum;
                        while (getline(rangeSplit, num, ',')) {
                            rangeNum.push_back(num);
                        }
                        sumAnomalies.emplace_back(stoi(rangeNum[0]), stoi(rangeNum[1]));
                        w = dio->read();
                    }
                    s = "Upload complete.\n";
                    dio->write(s);
                    // marge time steps reports
                    vector<pair<long, long>> sumReports;
                    string lastRep = " ";
                    long lastStep = 0;
                    for (AnomalyReport r: *ar) {
                        // case the reports are close
                        if (lastRep == r.description) {
                            if(lastStep + 1 == r.timeStep) {
                                long first = sumReports.back().first;
                                //pop the last range and push new range
                                sumReports.pop_back();
                                sumReports.emplace_back(first, r.timeStep);
                            } else {
                                sumReports.emplace_back(r.timeStep, r.timeStep);
                            }
                        }
                            // case new description report
                        else {
                            sumReports.emplace_back(r.timeStep, r.timeStep);
                        }
                        lastStep = r.timeStep;
                        lastRep = r.description;
                    }
                    // find false positive reports.
                    float TP = 0, FP = 0;
                    for (auto rep: sumReports) {
                        int flag = 0;
                        for (auto ano: sumAnomalies) {
                            // case the report had shared time with the anomaly
                            if (rep.first <= ano.first && rep.second >= ano.first){
                                flag = 1;
                            } else if (rep.first >= ano.first && rep.first <= ano.second){
                                flag = 1;
                            }
                        }
                        // case we didn't break so the report is false
                        if (0 == flag)
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
                    int N = numlines;
                    // write true positive rate
                    float tRate =(TP/P);
                    dio->write("True Positive Rate: " +  cutDecimal(tRate) + "\n");
                    // write false positive rate
                    float nRate =(FP/(N));
                    dio->write("False Positive Rate: " + cutDecimal(nRate) + "\n");
                    break;
                }
                */
        }
    }
}

CLI::~CLI() {
}
