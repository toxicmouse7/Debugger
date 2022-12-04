//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultCreateProcessDebugEventHandler.hpp"
#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"

#include <format>

void DefaultCreateProcessDebugEventHandler::HandleDebugEvent(const CreateProcessDebugEvent& event)
{
    if (event.payload.lpImageName != nullptr)
    {
        void* pImageName = ProcessMemoryManipulation::ReadPointer(
                event.processId, event.payload.lpImageName
        );

        if (event.payload.fUnicode == 0)
        {
            auto dllName = ProcessMemoryManipulation::ReadString(
                    event.processId, pImageName
            );
            LogCreateProcess(event.processId, event.threadId, dllName,
                             reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfImage),
                             reinterpret_cast<ULONG_PTR>(event.payload.lpStartAddress));
        }
        else
        {
            auto dllName = ProcessMemoryManipulation::ReadWString(
                    event.processId, pImageName
            );
            LogCreateProcess(event.processId, event.threadId, dllName,
                             reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfImage),
                             reinterpret_cast<ULONG_PTR>(event.payload.lpStartAddress));
        }
    }
    else
    {
        LogCreateProcess(event.processId, event.threadId, "(Unknown)",
                         reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfImage),
                         reinterpret_cast<ULONG_PTR>(event.payload.lpStartAddress));
    }
}

void DefaultCreateProcessDebugEventHandler::LogCreateProcess(DWORD pid, DWORD tid, const std::string& processName,
                                                             ULONG_PTR baseAddress, ULONG_PTR entryPoint) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Created process: " << processName << " at " << Utils::FormatAddress(baseAddress, "'");
        ss << std::dec << " (PID: " << pid << ", TID: " << tid << " Entry point: "
           << Utils::FormatAddress(entryPoint, "'") << ")";
        GetLogger().value().get().Log(ss.str());
    }
}

void DefaultCreateProcessDebugEventHandler::LogCreateProcess(DWORD pid, DWORD tid, const std::wstring& processName,
                                                             ULONG_PTR baseAddress, ULONG_PTR entryPoint) const
{
    if (GetLogger().has_value())
    {
        std::wstringstream ss;
        ss << L"Created process: " << processName << L" at " << Utils::FormatAddress(baseAddress, L"'");
        ss << std::dec << L" (PID: " << pid << L", TID: " << tid << L" Entry point: "
           << Utils::FormatAddress(entryPoint, L"'") << ")";
        GetLogger().value().get().Log(ss.str());
    }
}

DefaultCreateProcessDebugEventHandler::DefaultCreateProcessDebugEventHandler(
        std::optional<std::reference_wrapper<const Logger>> logger)
        : AbstractCreateProcessDebugEventHandler(logger)
{

}
