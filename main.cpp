#include "Debugger.hpp"

int main()
{
    Debugger* debugger = Debugger::GetInstance();

    debugger->StartAndAttach(R"(C:\Windows\System32\notepad.exe)");

    return 0;
}
