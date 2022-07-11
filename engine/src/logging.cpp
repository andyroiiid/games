//
// Created by Andrew Huang on 7/11/2022.
//

#include "logging.h"

std::mutex               g_logsMutex;
std::vector<std::string> g_logs;
