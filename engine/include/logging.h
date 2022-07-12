//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_LOG_H
#define GAMES_LOG_H

#include <mutex>
#include <vector>

extern std::mutex               g_logsMutex;
extern std::vector<std::string> g_logs;

template<class... Args>
static inline void LogInfo(const char *format, Args &&...args) {
    char buffer[1024];
    sprintf_s(buffer, format, args...);
    {
        std::lock_guard<std::mutex> lk(g_logsMutex);
        g_logs.emplace_back(buffer);
    }
}

template<class Func>
static inline void ForEachLog(Func &&f, size_t limit = 32) {
    std::lock_guard<std::mutex> lk(g_logsMutex);

    size_t numLogs   = std::min(g_logs.size(), limit);
    size_t iStartLog = g_logs.size() - numLogs;

    for (int i = 0; i < numLogs; i++) {
        f(i, g_logs[iStartLog + i]);
    }
}

#endif //GAMES_LOG_H
