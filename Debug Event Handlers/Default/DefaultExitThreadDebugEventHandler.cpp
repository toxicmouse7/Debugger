//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultExitThreadDebugEventHandler.hpp"

void DefaultExitThreadDebugEventHandler::HandleDebugEvent(const ExitThreadDebugEvent& event)
{
    RemoveThread(event.threadId);
    LogExitThread(event.processId, event.threadId, event.payload.dwExitCode);
}

void DefaultExitThreadDebugEventHandler::LogExitThread(DWORD pid, DWORD tid, DWORD exitCode) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Process " << pid << " exited thread " << tid << " with exit code " << exitCode;
        GetLogger().value().get().Log(ss.str());
    }
}
