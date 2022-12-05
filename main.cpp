#include "Debugger/Debugger.hpp"
#include "Debug Event Handlers/Default/DefaultHandlers.hpp"
#include "Logger/Logger.hpp"
#include "Logger/Message Handlers/MessageHandlers.hpp"
#include "Debug Event Handlers/TraceDebugEventHandler.hpp"

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

    debugger->SetErrorLogger(logger);

//    auto& debuggerState = debugger->CreateAndAttach(R"(C:\Windows\System32\notepad.exe)", false);
    auto& debuggerState = debugger->CreateAndAttach(
            R"(C:\Users\Aleksej\CLionProjects\test123\cmake-build-release\test.exe)", true);

    while (debuggerState != DebuggerState::Stopped)
    {
        auto exceptionContext = debugger->WaitForDebugEvent();

        if (exceptionContext.IsExternal() && exceptionContext.Type() != ExceptionContextType::eHardBreakpoint)
        {
            debugger->UseResolver<DefaultExceptionDebugEventHandler>(exceptionContext, logger);
        }
        else
        {
            debugger->UseResolver<TraceDebugEventHandler>(exceptionContext, logger);
        }
    }

    return 0;
}
