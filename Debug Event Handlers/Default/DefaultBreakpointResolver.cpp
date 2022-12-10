//
// Created by Aleksej on 06.12.2022.
//

#include "DefaultBreakpointResolver.hpp"

void DefaultBreakpointResolver::HandleDebugEvent(const ExceptionDebugEvent& event)
{
    if (exceptionContext.Type() == ExceptionContextType::eSoftBreakpoint)
    {
        CONTEXT context;
        context.ContextFlags = CONTEXT_ALL;
        auto thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, FALSE, event.threadId);
        if (thread == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error("Failed to open thread");
        }

        if (!GetThreadContext(thread, &context))
        {
            throw std::runtime_error("Failed to get thread context");
        }

        context.Rip -= 1;
        SetThreadContext(thread, &context);
    }
}

void DefaultBreakpointResolver::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                             ULONG_PTR exceptionAddress) const
{

}
