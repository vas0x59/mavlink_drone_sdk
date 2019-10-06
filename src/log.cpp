#include "log.h"

using namespace std;

namespace mavlink_drone_sdk
{

void LogInfo(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[1;32m"
         << "[INFO]" 
         << "\033[0;32m"
         << module 
         << "\033[0m" << ": " << str << "\033[0m\n";
}
void LogWarn(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[1;33m"
         << "[WARN]" 
         << "\033[0;33m" 
         << module 
         << "\033[0m" << ": " << str << "\033[0m\n";
}
void LogError(string module, string str)
{
    if (module != "")
        module = "[" + module + "]";
    cout << "\033[1;31m"
         << "[ERROR]" 
         << "\033[0;31m" 
         << module 
         << "\033[0m" << ": " << str << "\033[0m\n";
}
}; // namespace mavlink_drone_sdk