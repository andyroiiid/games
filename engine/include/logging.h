//
// Created by Andrew Huang on 7/11/2022.
//

#ifndef GAMES_LOG_H
#define GAMES_LOG_H

#include <mutex>
#include <deque>

static constexpr int LOG_MAX_CHARS = 512;

enum class LogLevel : uint8_t {
    Debug,
    Verbose,
    Info,
    Warning,
    Error,
};

struct LogEntry {
    LogLevel level;
    char     message[LOG_MAX_CHARS];
};

extern std::mutex           g_logsMutex;
extern std::deque<LogEntry> g_logs;

template<LogLevel level, class... Args>
static inline void Log(const char *format, Args &&...args) {
    LogEntry log{level};
    sprintf_s(log.message, format, args...);
    std::lock_guard<std::mutex> lk(g_logsMutex);
    g_logs.push_front(log);
}

#define LOG_FUNCTION_DEF(level) \
template<class... Args> \
static inline void Log##level(const char *format, Args &&...args) { \
    Log<LogLevel::level>(format, args...); \
}

LOG_FUNCTION_DEF(Debug)

LOG_FUNCTION_DEF(Verbose)

LOG_FUNCTION_DEF(Info)

LOG_FUNCTION_DEF(Warning)

LOG_FUNCTION_DEF(Error)

template<class Func>
static inline void IterateLatestLogs(Func &&func, size_t numLogs = 16) {
    std::lock_guard<std::mutex> lk(g_logsMutex);

    numLogs = std::min(g_logs.size(), numLogs);
    for (int i = 0; i < numLogs; i++)
        func(i, g_logs[i]);
}

static inline void ClearLogs() {
    g_logs.clear();
}

#endif //GAMES_LOG_H
