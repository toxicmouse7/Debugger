//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP


#include <Windows.h>
#include <string>

#include "Abstractions/Event Managing/AbstractEvent.hpp"
#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/DebugEventType.hpp"

class DefaultCreateProcessDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    static void LogProcessCreation(const std::string& processName, DWORD processId,
                                   DWORD threadId, ULONG_PTR startAddress,
                                   ULONG_PTR baseAddress);

    static void LogProcessCreation(const std::wstring& processName, DWORD processId,
                                   DWORD threadId, ULONG_PTR startAddress,
                                   ULONG_PTR baseAddress);

public:
    void Handle(const AbstractEvent<DebugEventType>& event) override;
};




#endif //DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP
