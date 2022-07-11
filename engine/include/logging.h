//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_LOG_H
#define GAMES_LOG_H

#include <cstdio>

template<class... Args>
static inline void LogInfo(const char *format, Args &&...args) {
    fprintf(stderr, format, args...);
}

#endif //GAMES_LOG_H
