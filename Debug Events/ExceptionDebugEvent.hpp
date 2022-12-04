//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_EXCEPTIONDEBUGEVENT_HPP
#define DEBUGGER_EXCEPTIONDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class ExceptionDebugEvent : public AbstractDebugEvent<EXCEPTION_DEBUG_INFO>
{
public:
    ExceptionDebugEvent(unsigned int threadId, unsigned int processId, const EXCEPTION_DEBUG_INFO& payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_EXCEPTIONDEBUGEVENT_HPP
