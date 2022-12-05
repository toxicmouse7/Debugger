//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_LOADDLLDEBUGEVENT_HPP
#define DEBUGGER_LOADDLLDEBUGEVENT_HPP

#include <Windows.h>

#include "Abstractions/Debug Events/AbstractDebugEvent.hpp"

class LoadDllDebugEvent : public AbstractDebugEvent<LOAD_DLL_DEBUG_INFO>
{
public:
    LoadDllDebugEvent(unsigned int threadId, unsigned int processId, LOAD_DLL_DEBUG_INFO payload)
            : AbstractDebugEvent(threadId, processId, payload) {}
};


#endif //DEBUGGER_LOADDLLDEBUGEVENT_HPP
