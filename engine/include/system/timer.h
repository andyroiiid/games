//
// Created by Andrew Huang on 7/10/2022.
//

#ifndef GAMES_TIMER_H
#define GAMES_TIMER_H

class Timer {
public:
    Timer();

    // returns the time (in seconds) since last tick (or construction)
    double tick();

private:
    long long prevTime = 0;
};

#endif //GAMES_TIMER_H
