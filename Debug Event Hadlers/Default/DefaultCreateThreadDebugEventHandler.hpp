//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP

#include <Windows.h>
#include <list>

#include "Debug Events/DebugEventType.hpp"
#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"

class DefaultCreateThreadDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    std::list<std::pair<DWORD, ULONG_PTR>>& threads;

    static void LogThreadCreation(DWORD threadId, ULONG_PTR startAddress);

    void AddThread(DWORD threadId, ULONG_PTR startAddress);

public:
    explicit DefaultCreateThreadDebugEventHandler(std::list<std::pair<DWORD, ULONG_PTR>>& threads);

    void Handle(const AbstractEvent<DebugEventType>& event) override;
};


#endif //DEBUGGER_DEFAULTCREATETHREADDEBUGEVENTHANDLER_HPP
