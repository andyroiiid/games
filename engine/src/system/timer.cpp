//
// Created by Andrew Huang on 7/10/2022.
//

#include "system/timer.h"

#include <Windows.h>

static long long GetPerformanceFrequency() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

static long long GetPerformanceCounter() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

Timer::Timer() : prevTime(GetPerformanceCounter()) {
}

double Timer::tick() {
    static const auto frequency = static_cast<double>(GetPerformanceFrequency());

    auto currTime  = GetPerformanceCounter();
    auto deltaTime = static_cast<double>(currTime - prevTime) / frequency;
    prevTime = currTime;

    return deltaTime;
}
