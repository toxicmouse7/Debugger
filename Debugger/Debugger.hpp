//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEBUGGER_HPP
#define DEBUGGER_DEBUGGER_HPP

#include <Windows.h>
#include <filesystem>
#include <memory>
#include <map>

#include "Abstractions/Event Managing/AbstractEventManager.hpp"
#include "Debug Event Handlers/DebugEventHandlerType.hpp"
#include "Debug Events/DebugEvents.hpp"
#include "Abstractions/Debug Event Handlers/AbstractDebugEventHandlers.hpp"
#include "Logger/Logger.hpp"
#include "ExceptionContext.hpp"
#include "Debug Event Handlers/Default/DefaultBreakpointOnStartHandler.hpp"
#include "Debug Event Handlers/Default/DefaultBreakpointResolver.hpp"

enum class DebuggerState
{
    NotStarted,
    Running,
    Paused,
    Stopped
};

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

class Debugger : public AbstractEventManager<DebugEventHandlerType>
{
private:
    inline static Debugger* instance = nullptr;
    HANDLE processHandle = nullptr;
    DebuggerState state = DebuggerState::NotStarted;
    std::map<std::wstring, ULONG_PTR> libraries;
    std::map<DWORD, ULONG_PTR> threads;
    std::list<std::shared_ptr<Breakpoint>> breakpoints;
    bool isWow64 = false;

    std::optional<std::reference_wrapper<const Logger>> errorLogger;

    std::shared_ptr<ExceptionDebugEvent> lastExceptionEvent = nullptr;

    Debugger()
    {
        if (instance != nullptr)
        {
            throw std::runtime_error("Debugger is already created");
        }
    }

    void OpenProcess(DWORD processId)
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

    void CreateDebugProcess(const std::filesystem::path& pathToExecutable)
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
                            DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
                            nullptr,
                            nullptr,
                            &startupInfo,
                            &processInformation)
                )
        {
            throw std::runtime_error("CreateProcess failed (" + std::to_string(GetLastError()) + ")");
        }

        BOOL isWow64Process = false;
        if (!IsWow64Process(processInformation.hProcess, &isWow64Process))
        {
            throw std::runtime_error("IsWow64Process failed (" + std::to_string(GetLastError()) + ")");
        }

        this->isWow64 = isWow64Process;

        this->processHandle = processInformation.hProcess;

        CloseHandle(processInformation.hThread);
    }

    void DebugLoop()
    {
        while (true)
        {
            DEBUG_EVENT debugEvent;
            if (!::WaitForDebugEvent(&debugEvent, INFINITE))
            {
                break;
            }

            try
            {
                auto debugEventHandlerType = DebugEventToDebugEventHandlerType(debugEvent);

                if (debugEventHandlerType == eException)
                {
                    lastExceptionEvent = std::dynamic_pointer_cast<ExceptionDebugEvent>(
                            DebugEventToAbstractEvent(debugEvent));
                    state = DebuggerState::Paused;
                    return;
                }

                auto abstractEvent = DebugEventToAbstractEvent(debugEvent);
                Notify(debugEventHandlerType, *abstractEvent);
            }
            catch (const std::exception& exception)
            {
                if (errorLogger.has_value())
                {
                    errorLogger.value().get().Log(exception.what());
                }
            }


            if (!ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, DBG_CONTINUE))
            {
                throw std::runtime_error("Failed to continue debug event");
            }
        }
    }

    void Notify(DebugEventHandlerType debugEventHandlerType, const AbstractEvent& event) override
    {
        if (recipients.contains(debugEventHandlerType))
        {
            for (auto& recipient: recipients[debugEventHandlerType])
            {
                recipient->Handle(event);
            }
        }
    }

    static std::shared_ptr<AbstractEvent> DebugEventToAbstractEvent(const DEBUG_EVENT& debugEvent)
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

    static DebugEventHandlerType DebugEventToDebugEventHandlerType(const DEBUG_EVENT& debugEvent)
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

    [[nodiscard]] std::shared_ptr<Breakpoint> FindBreakpoint(DWORD pid, ULONG_PTR address) const
    {
        for (auto& breakpoint: breakpoints)
        {
            if (breakpoint->GetProcessId() == pid && breakpoint->GetAddress() == address)
            {
                return breakpoint;
            }
        }

        return nullptr;
    }

    void SetBreakpointOnEntry()
    {
        AddRecipient(DebugEventHandlerType::eCreateProcess,
                     new DefaultBreakpointOnStartHandler([this](ULONG_PTR address)
                                                         {
                                                             SetBreakpoint(address);
                                                         }));
    }

    void SetTraceFlag()
    {
        auto thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, FALSE, lastExceptionEvent->threadId);
        if (thread == nullptr)
        {
            throw std::runtime_error("Failed to open thread");
        }

        CONTEXT context;
        context.ContextFlags = CONTEXT_ALL;
        if (!GetThreadContext(thread, &context))
        {
            throw std::runtime_error("Failed to get thread context");
        }

        context.EFlags |= 0x100;

        if (!SetThreadContext(thread, &context))
        {
            throw std::runtime_error("Failed to set thread context");
        }

        CloseHandle(thread);
    }

public:
    Debugger& operator=(const Debugger&) = delete;

    Debugger(const Debugger&) = delete;

    static Debugger* GetInstance()
    {
        {
            if (instance == nullptr)
            {
                instance = new Debugger();
            }
            return instance;
        }
    }

    const DebuggerState& CreateAndAttach(const std::filesystem::path& pathToExecutable)
    {
        if (processHandle != nullptr)
        {
            throw std::runtime_error("Some process is already opened");
        }

        CreateDebugProcess(pathToExecutable);
        return state;
    }

    void Attach(DWORD processId)
    {
        OpenProcess(processId);
        DebugActiveProcess(GetProcessId(processHandle));
        DebugLoop();
    }

    ExceptionContext WaitForDebugEvent()
    {
        if (state == DebuggerState::NotStarted || state == DebuggerState::Paused)
        {
            if (state == DebuggerState::Paused &&
                !ContinueDebugEvent(lastExceptionEvent->processId, lastExceptionEvent->threadId, DBG_CONTINUE))
            {
                throw std::runtime_error("Failed to continue debug event");
            }

            state = DebuggerState::Running;

            std::ranges::for_each(breakpoints, [](auto& breakpoint)
            {
                breakpoint->Enable();
            });
        }

        DebugLoop();

        bool isExternalBreakpoint = true;
        auto breakpoint = FindBreakpoint(lastExceptionEvent->processId,
                                         reinterpret_cast<ULONG_PTR>(lastExceptionEvent->payload
                                                 .ExceptionRecord.ExceptionAddress));

        if (breakpoint != nullptr)
        {
            isExternalBreakpoint = false;
        }

        std::ranges::for_each(breakpoints, [](auto& breakpoint)
        {
            breakpoint->Disable();
        });

        return {lastExceptionEvent, isExternalBreakpoint};
    }

    void WaitForEntry(bool trace)
    {
        if (state != DebuggerState::NotStarted)
        {
            throw std::runtime_error("Can't wait for entry when debugger is already started");
        }

        SetBreakpointOnEntry();
        while (true)
        {
            auto exceptionContext = WaitForDebugEvent();
            if (exceptionContext.IsExternal())
            {
                continue;
            }

            UseResolver<DefaultBreakpointResolver>(exceptionContext);
            if (trace)
            {
                SetTraceFlag();
            }

            break;
        }
        RemoveBreakpoint(
                FindBreakpoint(lastExceptionEvent->processId,
                               reinterpret_cast<ULONG_PTR>(lastExceptionEvent->payload
                                       .ExceptionRecord.ExceptionAddress))->GetAddress()
        );
    }

    std::shared_ptr<const Breakpoint> SetBreakpoint(ULONG_PTR address)
    {
        DWORD pid = GetProcessId(processHandle);
        auto breakpoint = std::make_shared<Breakpoint>(pid, address);

        breakpoints.push_back(breakpoint);

        return breakpoint;
    }

    void RemoveBreakpoint(ULONG_PTR address)
    {
        auto breakpoint = FindBreakpoint(GetProcessId(processHandle), address);

        if (breakpoint != nullptr)
        {
            breakpoints.remove(breakpoint);
        }
    }

    void SetErrorLogger(const Logger& logger)
    {
        errorLogger = logger;
    }

    [[nodiscard]] ULONG_PTR GetLibraryAddress(const std::wstring& libraryName) const
    {
        return libraries.at(libraryName);
    }

    [[nodiscard]] ULONG_PTR GetProcAddress(const std::wstring& libraryName, const std::string& functionName) const
    {

        return ProcessMemoryManipulation::GetProcAddressEx(processHandle, libraries.at(libraryName),
                                                           functionName, isWow64);
    }

    [[nodiscard]] bool IsWow64() const
    {
        return isWow64;
    }

    template<Derived<AbstractLoadDllDebugEventHandler> LoadDllHandler>
    void AddLoadDllHandler(const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        AddRecipient(DebugEventHandlerType::eLoadDll, new LoadDllHandler(libraries, logger));
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
        AddRecipient(DebugEventHandlerType::eUnloadDll, new UnloadDllHandler(libraries, logger));
    }

    template<Derived<AbstractExceptionDebugEventHandler> ExceptionHandler>
    void UseResolver(const ExceptionContext& exceptionContext,
                     const std::optional<std::reference_wrapper<const Logger>>& logger = std::nullopt)
    {
        ExceptionHandler(logger, exceptionContext).Handle(*lastExceptionEvent);
    }
};

#endif //DEBUGGER_DEBUGGER_HPP
