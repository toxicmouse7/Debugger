//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_UNLOADDLLDEBUGEVENT_HPP
#define DEBUGGER_UNLOADDLLDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class UnloadDllDebugEvent : public AbstractDebugEvent<UNLOAD_DLL_DEBUG_INFO>
{
public:
    UnloadDllDebugEvent(unsigned int threadId, unsigned int processId, UNLOAD_DLL_DEBUG_INFO payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_UNLOADDLLDEBUGEVENT_HPP
