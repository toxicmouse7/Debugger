//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEBUGEVENTTYPE_HPP
#define DEBUGGER_DEBUGEVENTTYPE_HPP


enum class DebugEventType
{
    eCreateProcess,
    eCreateThread,
    eExitProcess,
    eExitThread,
    eLoadDll,
    eUnloadDll,
    eOutputDebugString,
    eRtlRaiseException,
    eException,
    eBreakpoint,
    eSingleStep,
    eCreateProcessWOW,
    eLoadDllWOW,
    eUnloadDllWOW,
    eDebugString,
    eRipEvent
};


#endif //DEBUGGER_DEBUGEVENTTYPE_HPP
