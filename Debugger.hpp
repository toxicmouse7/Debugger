//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEBUGGER_HPP
#define DEBUGGER_DEBUGGER_HPP

#include <Windows.h>
#include <filesystem>
#include <memory>

#include "Abstractions/Event Managing/AbstractEventManager.hpp"
#include "Debug Event Handlers/DebugEventHandlerType.hpp"
#include "Debug Events/DebugEvents.hpp"
#include "Abstractions/Debug Event Handlers/AbstractDebugEventHandlers.hpp"
#include "Logger/Logger.hpp"

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

class Debugger : public AbstractEventManager<DebugEventHandlerType>
{
private:
    inline static Debugger* instance = nullptr;
    HANDLE processHandle = nullptr;
    std::list<std::pair<std::string, ULONG_PTR>> ansiLibraries;
    std::list<std::pair<std::wstring, ULONG_PTR>> unicodeLibraries;
    std::list<std::pair<DWORD, ULONG_PTR>> threads;

    Debugger();

    void OpenProcess(DWORD processId);

    void CreateDebugProcess(const std::filesystem::path& pathToExecutable);

    void DebugLoop();

    void Notify(DebugEventHandlerType debugEventHandlerType, const AbstractEvent& event) override;

    static std::shared_ptr<AbstractEvent> DebugEventToAbstractEvent(const DEBUG_EVENT& debugEvent);

    static DebugEventHandlerType DebugEventToDebugEventHandlerType(const DEBUG_EVENT& debugEvent);

public:
    Debugger& operator=(const Debugger&) = delete;

    Debugger(const Debugger&) = delete;

    static Debugger* GetInstance();

    void CreateAndAttach(const std::filesystem::path& pathToExecutable);

    void Attach(DWORD processId);

    template<Derived<AbstractLoadDllDebugEventHandler> LoadDllHandler>
    void AddLoadDllHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eLoadDll, new LoadDllHandler(ansiLibraries, unicodeLibraries, logger));
    }

    template<Derived<AbstractCreateProcessDebugEventHandler> CreateProcessHandler>
    void AddCreateProcessHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eCreateProcess, new CreateProcessHandler(logger));
    }

    template<Derived<AbstractCreateThreadDebugEventHandler> CreateThreadHandler>
    void AddCreateThreadHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eCreateThread, new CreateThreadHandler(threads, logger));
    }

    template<Derived<AbstractExitThreadDebugEventHandler> ExitThreadHandler>
    void AddExitThreadHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eExitThread, new ExitThreadHandler(threads, logger));
    }

    template<Derived<AbstractExitProcessDebugEventHandler> ExitProcessHandler>
    void AddExitProcessHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eExitProcess, new ExitProcessHandler(logger));
    }

    template<Derived<AbstractUnloadDllDebugEventHandler> UnloadDllHandler>
    void AddUnloadDllHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eUnloadDll, new UnloadDllHandler(ansiLibraries, unicodeLibraries, logger));
    }

    template<Derived<AbstractExceptionDebugEventHandler> ExceptionHandler>
    void AddExceptionHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eUnloadDll, new ExceptionHandler(logger));
    }
};

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
                        DEBUG_PROCESS | CREATE_NEW_CONSOLE,
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

void Debugger::CreateAndAttach(const std::filesystem::path& pathToExecutable)
{
    CreateDebugProcess(pathToExecutable);
    DebugLoop();
}

void Debugger::Attach(DWORD processId)
{
    OpenProcess(processId);
    DebugActiveProcess(GetProcessId(processHandle));
    DebugLoop();
}

void Debugger::DebugLoop()
{
    while (true)
    {
        DEBUG_EVENT debugEvent;
        if (!WaitForDebugEvent(&debugEvent, INFINITE))
        {
            break;
        }

        try
        {
            auto debugEventHandlerType = DebugEventToDebugEventHandlerType(debugEvent);;
            auto abstractEvent = DebugEventToAbstractEvent(debugEvent);
            Notify(debugEventHandlerType, *abstractEvent);
        }
        catch (const std::exception& exception)
        {
//            std::cout << exception.what() << std::endl;
        }


        if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE))
        {
            throw std::runtime_error("Failed to continue debug event");
        }
    }
}

void Debugger::Notify(DebugEventHandlerType debugEventHandlerType, const AbstractEvent& event)
{
    if (recipients.contains(debugEventHandlerType))
    {
        for (auto& recipient: recipients[debugEventHandlerType])
        {
            recipient->Handle(event);
        }
    }
}

Debugger::Debugger()
{
    if (instance != nullptr)
    {
        throw std::runtime_error("Debugger is already created");
    }
}

std::shared_ptr<AbstractEvent> Debugger::DebugEventToAbstractEvent(const DEBUG_EVENT& debugEvent)
{
    switch (debugEvent.dwDebugEventCode)
    {
        case CREATE_PROCESS_DEBUG_EVENT:
            return std::make_shared<CreateProcessDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                             debugEvent.u.CreateProcessInfo);
        case CREATE_THREAD_DEBUG_EVENT:
            return std::make_shared<CreateThreadDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                            debugEvent.u.CreateThread);
        case EXCEPTION_DEBUG_EVENT:
            return std::make_shared<ExceptionDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                         debugEvent.u.Exception);
        case EXIT_PROCESS_DEBUG_EVENT:
            return std::make_shared<ExitProcessDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                           debugEvent.u.ExitProcess);
        case EXIT_THREAD_DEBUG_EVENT:
            return std::make_shared<ExitThreadDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                          debugEvent.u.ExitThread);
        case LOAD_DLL_DEBUG_EVENT:
            return std::make_shared<LoadDllDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                       debugEvent.u.LoadDll);
//        case OUTPUT_DEBUG_STRING_EVENT:
//            return AbstractDebugEvent(debugEvent);
//        case RIP_EVENT:
//            return AbstractDebugEvent(debugEvent);
        case UNLOAD_DLL_DEBUG_EVENT:
            return std::make_shared<UnloadDllDebugEvent>(debugEvent.dwThreadId, debugEvent.dwProcessId,
                                                         debugEvent.u.UnloadDll);
        default:
            throw std::runtime_error("Unknown debug event");
    }
}

DebugEventHandlerType Debugger::DebugEventToDebugEventHandlerType(const DEBUG_EVENT& debugEvent)
{
    switch (debugEvent.dwDebugEventCode)
    {
        case CREATE_PROCESS_DEBUG_EVENT:
            return DebugEventHandlerType::eCreateProcess;
        case CREATE_THREAD_DEBUG_EVENT:
            return DebugEventHandlerType::eCreateThread;
        case EXCEPTION_DEBUG_EVENT:
            return DebugEventHandlerType::eException;
        case EXIT_PROCESS_DEBUG_EVENT:
            return DebugEventHandlerType::eExitProcess;
        case EXIT_THREAD_DEBUG_EVENT:
            return DebugEventHandlerType::eExitThread;
        case LOAD_DLL_DEBUG_EVENT:
            return DebugEventHandlerType::eLoadDll;
//        case OUTPUT_DEBUG_STRING_EVENT:
//            return DebugEventHandlerType::eOutputDebugString;
//        case RIP_EVENT:
//            return DebugEventHandlerType::eRip;
        case UNLOAD_DLL_DEBUG_EVENT:
            return DebugEventHandlerType::eUnloadDll;
        default:
            throw std::runtime_error("Unknown debug event");
    }
}

#endif //DEBUGGER_DEBUGGER_HPP
