//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEBUGEVENT_HPP
#define DEBUGGER_DEBUGEVENT_HPP

#include <Windows.h>

#include "DebugEventType.hpp"
#include "Abstractions/Event Managing/AbstractEvent.hpp"

class DebugEvent : public AbstractEvent<DebugEventType>
{
private:
    static DebugEventType GetEventType(const DEBUG_EVENT& event);
    static const void* GetDebugEventData(const DEBUG_EVENT& event);

public:
    HANDLE processHandle;
    explicit DebugEvent(const DEBUG_EVENT& event, HANDLE processHandle);
};


#endif //DEBUGGER_DEBUGEVENT_HPP
