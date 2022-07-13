//
// Created by Andrew Huang on 7/11/2022.
//

#include "logging.h"

std::mutex           g_logsMutex;
std::deque<LogEntry> g_logs; // NOLINT(cert-err58-cpp)
