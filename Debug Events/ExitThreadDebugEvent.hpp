//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_EXITTHREADDEBUGEVENT_HPP
#define DEBUGGER_EXITTHREADDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class ExitThreadDebugEvent : public AbstractDebugEvent<EXIT_THREAD_DEBUG_INFO>
{
public:
    ExitThreadDebugEvent(unsigned int threadId, unsigned int processId, EXIT_THREAD_DEBUG_INFO payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_EXITTHREADDEBUGEVENT_HPP
