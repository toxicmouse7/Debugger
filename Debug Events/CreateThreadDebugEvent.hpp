//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_CREATETHREADDEBUGEVENT_HPP
#define DEBUGGER_CREATETHREADDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class CreateThreadDebugEvent : public AbstractDebugEvent<CREATE_THREAD_DEBUG_INFO>
{
public:
    CreateThreadDebugEvent(unsigned int threadId, unsigned int processId, const CREATE_THREAD_DEBUG_INFO& payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_CREATETHREADDEBUGEVENT_HPP
