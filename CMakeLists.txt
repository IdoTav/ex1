cmake_minimum_required(VERSION 3.20)
project(untitled7)

set(CMAKE_CXX_STANDARD 11)

add_executable(untitled7 anomaly_detection_util.cpp anomaly_detection_util.h timeseries.cpp timeseries.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h AnomalyDetector.h MainTrain.cpp minCircle.h minCircle.cpp
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.h CLI.cpp commands.h)
