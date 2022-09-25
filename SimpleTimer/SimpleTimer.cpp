//
// Created by Евгений on 25.09.2022.
//

#include "SimpleTimer.h"
#include <iostream>

SimpleTimer::SimpleTimer() {
    this->start = std::chrono::high_resolution_clock::now();
}

SimpleTimer::~SimpleTimer() {
    this->end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = this->end - this->start;
    std::cout << "Duration " << duration.count() << " s" << std::endl;
}