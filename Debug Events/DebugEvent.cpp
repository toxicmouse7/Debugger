//
// Created by Aleksej on 02.12.2022.
//

#include "DebugEvent.hpp"

DebugEvent::DebugEvent(const DEBUG_EVENT& event, HANDLE processHandle)
        : AbstractEvent<DebugEventType>(GetEventType(event),
                                        GetDebugEventData(event)),
          processHandle(processHandle)
{
}

DebugEventType DebugEvent::GetEventType(const DEBUG_EVENT& event)
{
    switch (event.dwDebugEventCode)
    {
        case EXCEPTION_DEBUG_EVENT:
            return DebugEventType::eException;
        case CREATE_THREAD_DEBUG_EVENT:
            return DebugEventType::eCreateThread;
        case CREATE_PROCESS_DEBUG_EVENT:
            return DebugEventType::eCreateProcess;
        case EXIT_THREAD_DEBUG_EVENT:
            return DebugEventType::eExitThread;
        case EXIT_PROCESS_DEBUG_EVENT:
            return DebugEventType::eExitProcess;
        case LOAD_DLL_DEBUG_EVENT:
            return DebugEventType::eLoadDll;
        case UNLOAD_DLL_DEBUG_EVENT:
            return DebugEventType::eUnloadDll;
        case OUTPUT_DEBUG_STRING_EVENT:
            return DebugEventType::eOutputDebugString;
        case RIP_EVENT:
            return DebugEventType::eRipEvent;
        default:
            return DebugEventType::eException;
    }
}

const void* DebugEvent::GetDebugEventData(const DEBUG_EVENT& event)
{
    switch (event.dwDebugEventCode)
    {
        case EXCEPTION_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.Exception);
        case CREATE_THREAD_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.CreateThread);
        case CREATE_PROCESS_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.CreateProcessInfo);
        case EXIT_THREAD_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.ExitThread);
        case EXIT_PROCESS_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.ExitProcess);
        case LOAD_DLL_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.LoadDll);
        case UNLOAD_DLL_DEBUG_EVENT:
            return static_cast<const void*>(&event.u.UnloadDll);
        case OUTPUT_DEBUG_STRING_EVENT:
            return static_cast<const void*>(&event.u.DebugString);
        case RIP_EVENT:
            return static_cast<const void*>(&event.u.RipInfo);
        default:
            return nullptr;
    }
}