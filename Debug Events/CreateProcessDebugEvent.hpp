//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_CREATEPROCESSDEBUGEVENT_HPP
#define DEBUGGER_CREATEPROCESSDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class CreateProcessDebugEvent : public AbstractDebugEvent<CREATE_PROCESS_DEBUG_INFO>
{
public:
    CreateProcessDebugEvent(unsigned int threadId, unsigned int processId, const CREATE_PROCESS_DEBUG_INFO & payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};

#endif //DEBUGGER_CREATEPROCESSDEBUGEVENT_HPP
