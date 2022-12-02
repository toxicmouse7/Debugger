//
// Created by Aleksej on 02.12.2022.
//

export module DebugEventType;


export enum class DebugEventType
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
