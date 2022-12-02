//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEBUGGER_HPP
#define DEBUGGER_DEBUGGER_HPP

#include <Windows.h>
#include <filesystem>

#include "Debug Events/DebugEvent.hpp"
#include "Debug Events/DebugEventType.hpp"
#include "Abstractions/Event Managing/AbstractEventManager.hpp"

class Debugger : public AbstractEventManager<DebugEventType>
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

    void Run();

    void NotifyAll(const AbstractEvent<DebugEventType>& event) override;

public:
    Debugger& operator=(const Debugger&) = delete;

    Debugger(const Debugger&) = delete;

    static Debugger* GetInstance();

    void StartAndAttach(const std::filesystem::path& pathToExecutable);

    void Attach(DWORD processId);
};


#endif //DEBUGGER_DEBUGGER_HPP
