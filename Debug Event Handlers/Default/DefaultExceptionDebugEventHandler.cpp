//
// Created by Aleksej on 04.12.2022.
//

#include "DefaultExceptionDebugEventHandler.hpp"

void DefaultExceptionDebugEventHandler::HandleDebugEvent(const ExceptionDebugEvent& event)
{
    LogException(event.processId, ExceptionCodeToString(event.payload.ExceptionRecord.ExceptionCode),
                 event.payload.ExceptionRecord.ExceptionCode,
                 reinterpret_cast<ULONG_PTR>(event.payload.ExceptionRecord.ExceptionAddress));
}

void DefaultExceptionDebugEventHandler::HandleBreakpoint(const ExceptionDebugEvent& event)
{
    LogBreakpoint();
}

void DefaultExceptionDebugEventHandler::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                                     ULONG_PTR exceptionAddress) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Process " << pid << " encountered exception " << exceptionName << " with code " << exceptionCode
           << " at address " << Utils::FormatAddress(exceptionAddress, "'");
        GetLogger().value().get().Log(ss.str());
    }
}

void DefaultExceptionDebugEventHandler::LogBreakpoint() const
{
    if (GetLogger().has_value())
    {
        GetLogger().value().get().Log("Breakpoint hit");
    }
}
