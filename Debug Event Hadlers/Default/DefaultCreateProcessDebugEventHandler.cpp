//
// Created by Aleksej on 02.12.2022.
//

#include <sstream>

#include "DefaultCreateProcessDebugEventHandler.hpp"
#include "Logger/Logger.hpp"
#include "Process Memory Manipultion/ProcessMemoryManipulation.hpp"
#include "Debug Events/DebugEvent.hpp"

void DefaultCreateProcessDebugEventHandler::Handle(const AbstractEvent<DebugEventType>& event)
{
    if (event.eventType != DebugEventType::eCreateProcess)
        return;

    auto createProcessEvent = dynamic_cast<const DebugEvent&>(event);

    auto createProcessData = static_cast<const CREATE_PROCESS_DEBUG_INFO*>(event.GetData());

    if (createProcessData->lpImageName != nullptr)
    {
        if (createProcessData->fUnicode == 0)
        {
            auto processName = ProcessMemoryManipulation::ReadString(
                    createProcessEvent.processHandle, createProcessData->lpImageName
            );
            LogProcessCreation(processName, GetProcessId(createProcessData->hProcess),
                               GetThreadId(createProcessData->hThread),
                               reinterpret_cast<ULONG_PTR>(createProcessData->lpStartAddress),
                               reinterpret_cast<ULONG_PTR>(createProcessData->lpBaseOfImage)
            );
        }
        else
        {
            auto processName = ProcessMemoryManipulation::ReadWString(
                    createProcessEvent.processHandle, createProcessData->lpImageName
            );
            LogProcessCreation(processName, GetProcessId(createProcessData->hProcess),
                               GetThreadId(createProcessData->hThread),
                               reinterpret_cast<ULONG_PTR>(createProcessData->lpStartAddress),
                               reinterpret_cast<ULONG_PTR>(createProcessData->lpBaseOfImage)
            );
        }
    }
    else
    {
        LogProcessCreation("(Unknown)", GetProcessId(createProcessData->hProcess),
                           GetThreadId(createProcessData->hThread),
                           reinterpret_cast<ULONG_PTR>(createProcessData->lpStartAddress),
                           reinterpret_cast<ULONG_PTR>(createProcessData->lpBaseOfImage)
        );
    }
}

void DefaultCreateProcessDebugEventHandler::LogProcessCreation(const std::string& processName, DWORD processId,
                                                               DWORD threadId, ULONG_PTR startAddress,
                                                               ULONG_PTR baseAddress)
{
    std::stringstream ss;
    ss << "Process " << processName << " created with id " << processId;
    ss << " and thread id " << threadId;
    ss << " (Base address: " << std::hex << baseAddress << ' ';
    ss << "Start address: " << std::hex << startAddress << ')';
    Logger::GetInstance()->Log(ss.str());
}

void DefaultCreateProcessDebugEventHandler::LogProcessCreation(const std::wstring& processName, DWORD processId,
                                                               DWORD threadId, ULONG_PTR startAddress,
                                                               ULONG_PTR baseAddress)
{
    std::wstringstream ss;
    ss << "Process " << processName << " created with id " << processId;
    ss << " and thread id " << threadId;
    ss << " (Start address: " << std::hex << startAddress << ')';
    Logger::GetInstance()->Log(ss.str());
}
