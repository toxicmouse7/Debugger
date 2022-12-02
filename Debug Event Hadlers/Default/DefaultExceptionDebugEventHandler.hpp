//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP

#include <Windows.h>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Abstractions/Event Managing/AbstractEvent.hpp"
#include "Debug Events/DebugEventType.hpp"

class DefaultExceptionDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    void Handle(const AbstractEvent<DebugEventType>& event) override;
};

void DefaultExceptionDebugEventHandler::Handle(const AbstractEvent<DebugEventType>& event)
{
    if (event.eventType != DebugEventType::eException)
        return;

    auto exceptionData = static_cast<const EXCEPTION_DEBUG_INFO*>(event.GetData());
}


#endif //DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP
