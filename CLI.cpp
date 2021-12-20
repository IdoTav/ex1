#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    dio = dio;
    TimeSeries* trainTs;
    TimeSeries* testTs;
    HybridAnomalyDetector* ad;
    vector<AnomalyReport>* r;
}


void printMenu() {
    std::cout << "Welcome to the Anomaly Detection Server.\n"
               "Please choose an option:\n"
               "1. upload a time series csv file\n"
               "2. algorithm settings\n"
               "3. detect anomalies\n"
               "4. display results\n"
               "5. upload anomalies and analyze results\n"
               "6. exit" << std::endl;
}

void CLI::start(){
    printMenu();
    int i = 0;
    int* N;
    while(true) {
        std::cin >> i;
        if (6 == i)
            break;
        switch (i) {
            case(1): {
                uploadAtimeSeriesCommand t(dio, trainTs, testTs, ad, r, N);
                t.execute();
                break;
            }
            case(2): {
                currentThresholdCommand c(dio, trainTs, testTs, ad, r, N);
                c.execute();
                break;
            }
            case(3): {
                detectAnomaliesCommand d(dio, trainTs, testTs, ad, r, N);
                d.execute();
                break;
            }
            case(4): {
                displayCommand ds (dio, trainTs, testTs, ad, r, N);
                ds.execute();
                break;
            }
            case(5): {
                break;
            }
        }
    }

}


CLI::~CLI() {
}

