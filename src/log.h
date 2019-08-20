#pragma once

#include <string>
#include <iostream>

using namespace std;

namespace mavlink_indoor_sdk
{
void LogInfo(string module, string str);
void LogWarn(string module, string str);
void LogError(string module, string str);
}; // namespace mavlink_indoor_sdk