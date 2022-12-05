//
// Created by Aleksej on 04.12.2022.
//

#include "DefaultExceptionDebugEventHandler.hpp"

#include <iomanip>

void DefaultExceptionDebugEventHandler::HandleDebugEvent(const ExceptionDebugEvent& event)
{
    LogException(event.processId, ExceptionCodeToString(event.payload.ExceptionRecord.ExceptionCode),
                 event.payload.ExceptionRecord.ExceptionCode,
                 reinterpret_cast<ULONG_PTR>(event.payload.ExceptionRecord.ExceptionAddress));


}

void DefaultExceptionDebugEventHandler::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                                     ULONG_PTR exceptionAddress) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Process " << pid << " encountered exception " << std::quoted(exceptionName);
        ss << " with code " << std::hex << exceptionCode;
        ss << " at address " << Utils::FormatAddress(exceptionAddress, "'");
        GetLogger().value().get().Log(ss.str());
    }
}
