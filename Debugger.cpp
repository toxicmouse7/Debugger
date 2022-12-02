//
// Created by Aleksej on 02.12.2022.
//

#include "Debugger.hpp"

#include "Logger/Logger.hpp"
#include "Logger/Message Handlers/ConsoleMessageHandler.hpp"
#include "Debug Event Hadlers/Default/DefaultExceptionDebugEventHandler.hpp"
#include "Debug Event Hadlers/Default/DefaultCreateProcessDebugEventHandler.hpp"
#include "Debug Event Hadlers/Default/DefaultLoadDllDebugEventHandler.hpp"
#include "Debug Event Hadlers/Default/DefaultCreateThreadDebugEventHandler.hpp"

Debugger* Debugger::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Debugger();
    }
    return instance;
}

void Debugger::OpenProcess(DWORD processId)
{
    if (processHandle != nullptr)
    {
        throw std::runtime_error("Some process is already opened");
    }

    processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == nullptr)
    {
        throw std::runtime_error("Failed to open process");
    }
}

void Debugger::CreateDebugProcess(const std::filesystem::path& pathToExecutable)
{
    if (processHandle != nullptr)
    {
        throw std::runtime_error("Some process is already opened");
    }

    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInformation;

    ZeroMemory(&processInformation, sizeof(processInformation));
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;
    startupInfo.wShowWindow = SW_SHOWNORMAL;

    // Start the child process.
    if (!CreateProcessW(pathToExecutable.c_str(),
                        nullptr,
                        nullptr,
                        nullptr,
                        false,
                        DEBUG_ONLY_THIS_PROCESS,
                        nullptr,
                        nullptr,
                        &startupInfo,
                        &processInformation)
            )
    {
        throw std::runtime_error("CreateProcess failed (" + std::to_string(GetLastError()) + ")");
    }

    this->processHandle = processInformation.hProcess;

    CloseHandle(processInformation.hThread);
}

void Debugger::StartAndAttach(const std::filesystem::path& pathToExecutable)
{
    CreateDebugProcess(pathToExecutable);
    Run();
}

void Debugger::Attach(DWORD processId)
{
    OpenProcess(processId);
    DebugActiveProcess(GetProcessId(processHandle));
    Run();
}

void Debugger::Run()
{
    while (true)
    {
        DEBUG_EVENT debugEvent;
        if (!WaitForDebugEvent(&debugEvent, INFINITE))
        {
            break;
        }

        NotifyAll(DebugEvent(debugEvent, processHandle));

        if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE))
        {
            throw std::runtime_error("Failed to continue debug event");
        }
    }
}

void Debugger::NotifyAll(const AbstractEvent<DebugEventType>& event)
{
    for (auto recipient : recipients)
    {
        recipient->Handle(event);
    }
}

Debugger::Debugger()
{
    if (instance != nullptr)
    {
        throw std::runtime_error("Debugger is already created");
    }

    Logger::GetInstance()->AddMessageHandler(new ConsoleMessageHandler());

    recipients.push_back(new DefaultExceptionDebugEventHandler());
    recipients.push_back(new DefaultCreateProcessDebugEventHandler());
    recipients.push_back(new DefaultLoadDllDebugEventHandler(ansiLibraries, unicodeLibraries));
    recipients.push_back(new DefaultCreateThreadDebugEventHandler(threads));
}