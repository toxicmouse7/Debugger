//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractCreateThreadDebugEventHandler.hpp"

class DefaultCreateThreadDebugEventHandler : public AbstractCreateThreadDebugEventHandler
{
private:
    void HandleDebugEvent(const CreateThreadDebugEvent& event) override;

    void LogCreateThread(DWORD tid, ULONG_PTR startAddress) const override;

public:

    explicit DefaultCreateThreadDebugEventHandler(std::list<std::pair<DWORD, ULONG_PTR>>& threads,
                                                  std::optional<std::reference_wrapper<const Logger>> logger);

    ~DefaultCreateThreadDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP
