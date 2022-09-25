//
// Created by Евгений on 25.09.2022.
//

#ifndef FILEFINDER_SIMPLETIMER_H
#define FILEFINDER_SIMPLETIMER_H
#include <chrono>

class SimpleTimer {
private:
    std::chrono::high_resolution_clock::time_point start,end;
public:
    SimpleTimer();
    ~SimpleTimer();
};


#endif //FILEFINDER_SIMPLETIMER_H
