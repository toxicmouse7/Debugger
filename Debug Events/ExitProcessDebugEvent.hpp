//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_EXITPROCESSDEBUGEVENT_HPP
#define DEBUGGER_EXITPROCESSDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class ExitProcessDebugEvent : public AbstractDebugEvent<EXIT_PROCESS_DEBUG_INFO>
{
public:
    ExitProcessDebugEvent(unsigned int threadId, unsigned int processId, const EXIT_PROCESS_DEBUG_INFO & payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_EXITPROCESSDEBUGEVENT_HPP
