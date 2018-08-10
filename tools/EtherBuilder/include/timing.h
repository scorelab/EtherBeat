/*
 * Created by prabushitha on 7/1/18.
 * Copyright [2018] <ScoreLab Organization>
*/
#ifndef TOOLS_ETHERBUILDER_INCLUDE_TIMING_H_
#define TOOLS_ETHERBUILDER_INCLUDE_TIMING_H_

#include <iostream>
#include <cstdio>
#include <chrono>
#include <string>

class TimeCalculator {
 public:
    int64_t timeStart;
    int64_t getCurrentTimeMilli() {
        auto ms = std::chrono::duration_cast< std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch());

        return ms.count();
    }

    void setStart() {
        timeStart = getCurrentTimeMilli();
    }



    void printElapsedTime() {
        int64_t timeEnd = getCurrentTimeMilli();
        std::cout << "Time taken = " << timeEnd-timeStart << " ms" << std::endl;
    }
};

#endif  // TOOLS_ETHERBUILDER_INCLUDE_TIMING_H_
