#include "log.h"

using namespace std;

namespace mavlink_indoor_sdk
{

void LogInfo(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[0;32m"
         << "[INFO]" << module << ": " << str << "\033[0m\n";
}
void LogWarn(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[0;33m"
         << "[WARN]" << module << ": " << str << "\033[0m\n";
}
void LogError(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[0;31m"
         << "[ERROR]" << module << ": " << str << "\033[0m\n";
}
}; // namespace mavlink_indoor_sdk