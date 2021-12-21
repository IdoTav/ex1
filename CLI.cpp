#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->ad = HybridAnomalyDetector();
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
    *trainTs = tmp;
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
    testTs = &tmp;
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

void CLI::start(){
    TimeSeries tmp1, tmp;
    vector<AnomalyReport> ar;
    int numlines = -1;
    while(true) {
        printMenu(this->dio);
        string index = this->dio->read();
        int i = stoi(index);
        if (6 == i)
            break;
        switch (i) {
            case(1): {
                string s = "Please upload your local train CSV file.\n";
                dio->write(s);
                std::fstream serverFile;
                serverFile.open("anomalyTrain.csv", fstream ::app);
                string w = dio->read();
                while (w != "done") {
                    serverFile << w + "\n";
                    w = dio->read();
                    numlines += 1;
                }
                serverFile.close();
                s = "Upload complete.\n";
                dio->write(s);
                tmp = TimeSeries("anomalyTrain.csv");
                trainTs = &tmp;
                s = "Please upload your local test CSV file.\n";
                dio->write(s);
                std::fstream serverFile1;
                serverFile1.open("anomalyTest.csv", fstream ::app);
                w = dio->read();
                while (w != "done") {
                    serverFile1 << w + "\n";
                    w = dio->read();
                }
                serverFile1.close();
                s = "Upload complete.\n";
                dio->write(s);
                tmp1 = TimeSeries("anomalyTest.csv");
                testTs = &tmp1;
                break;
            }
            case(2): {
                string s = "The current correlation threshold is " + to_string(ad.getTopThreshold()) + "\n";
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
                ad.setTopThreshold(newThreshold);
                break;
            }
            case(3): {

                ad.learnNormal(*trainTs);
                ar = ad.detect(*testTs);
                r = &ar;
                string s = "anomaly detection complete.\n";
                dio->write(s);
                break;
            }
            case(4): {
                for (AnomalyReport anoR : *r) {
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
                /**
                int tmpRate = tRate * 1000;
                tRate = (float) tmpRate / 1000;**/
                dio->write("True Positive Rate: " +  cutDecimal(tRate) + "\n");
                // write false positive rate
                float nRate =(FP/(N));
                /**
                tmpRate = nRate * 1000;
                nRate = (float) tmpRate / 1000;
                 **/
                dio->write("False Positive Rate: " + cutDecimal(nRate) + "\n");
                break;
            }
        }
    }

}

CLI::~CLI() {
}
