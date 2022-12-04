//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTEXITPROCESSDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTEXITPROCESSDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExitProcessDebugEventHandler.hpp"

class DefaultExitProcessDebugEventHandler : public AbstractExitProcessDebugEventHandler
{
private:
    void HandleDebugEvent(const ExitProcessDebugEvent& event) override;

    void LogExitProcess(DWORD pid, DWORD exitCode) const override;

public:
    explicit DefaultExitProcessDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger)
            : AbstractExitProcessDebugEventHandler(logger) {}

    ~DefaultExitProcessDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTEXITPROCESSDEBUGEVENTHANDLER_HPP
