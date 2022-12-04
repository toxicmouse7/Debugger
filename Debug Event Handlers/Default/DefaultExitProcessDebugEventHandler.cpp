//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultExitProcessDebugEventHandler.hpp"

void DefaultExitProcessDebugEventHandler::HandleDebugEvent(const ExitProcessDebugEvent& event)
{
    LogExitProcess(event.processId, event.payload.dwExitCode);
}

void DefaultExitProcessDebugEventHandler::LogExitProcess(DWORD pid, DWORD exitCode) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Process " << pid << " exited with code " << exitCode;
        GetLogger().value().get().Log(ss.str());
    }
}
