//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTEXITTHREADDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTEXITTHREADDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExitThreadDebugEventHandler.hpp"

class DefaultExitThreadDebugEventHandler : public AbstractExitThreadDebugEventHandler
{
private:
    void HandleDebugEvent(const ExitThreadDebugEvent& event) override;

    void LogExitThread(DWORD pid, DWORD tid, DWORD exitCode) const override;

public:
    explicit DefaultExitThreadDebugEventHandler(std::map<DWORD, ULONG_PTR>& threads,
                                                std::optional<std::reference_wrapper<const Logger>> logger)
                                                : AbstractExitThreadDebugEventHandler(threads, logger) {}

    ~DefaultExitThreadDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTEXITTHREADDEBUGEVENTHANDLER_HPP
