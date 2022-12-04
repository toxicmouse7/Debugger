#include "Debugger.hpp"
#include "Debug Event Handlers/Default/DefaultHandlers.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Message Handlers/MessageHandlers.hpp"

int main()
{
    Debugger* debugger = Debugger::GetInstance();
    auto& logger = Logger::GetInstance("Debugger");

    logger.AddMessageHandler(new ConsoleMessageHandler());

    debugger->AddLoadDllHandler<DefaultLoadDllDebugEventHandler>(logger);
    debugger->AddCreateProcessHandler<DefaultCreateProcessDebugEventHandler>(logger);
    debugger->AddUnloadDllHandler<DefaultUnloadDllDebugEventHandler>(logger);
    debugger->AddExitProcessHandler<DefaultExitProcessDebugEventHandler>(logger);
    debugger->AddExitThreadHandler<DefaultExitThreadDebugEventHandler>(logger);
    debugger->AddCreateThreadHandler<DefaultCreateThreadDebugEventHandler>(logger);
    debugger->AddExceptionHandler<DefaultExceptionDebugEventHandler>(logger);

//    debugger->CreateAndAttach(R"(C:\Windows\System32\notepad.exe)");
    debugger->CreateAndAttach(R"(C:\Users\Aleksej\CLionProjects\test123\cmake-build-release\test.exe)");

    return 0;
}
