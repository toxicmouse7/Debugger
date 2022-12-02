//
// Created by Aleksej on 02.12.2022.
//

module;

#include <Windows.h>

export module DefaultExceptionDebugEventHandler;

import AbstractEventRecipient;
import DebugEvent;

export class DefaultExceptionDebugEventHandler : public AbstractEventRecipient<DebugEventType>
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
