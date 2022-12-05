//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_EXCEPTIONCONTEXT_HPP
#define DEBUGGER_EXCEPTIONCONTEXT_HPP

#include <memory>
#include <string>

#include "Debug Events/ExceptionDebugEvent.hpp"

enum class ExceptionContextType
{
    eSoftBreakpoint,
    eHardBreakpoint,
    eException
};

class ExceptionContext
{
private:
    bool isExternal;
    ExceptionContextType exceptionContextType;
public:
    ExceptionContext(const std::shared_ptr<ExceptionDebugEvent>& event, bool isExternal)
        : isExternal(isExternal)
    {
        switch (event->payload.ExceptionRecord.ExceptionCode)
        {
            case EXCEPTION_BREAKPOINT:
            case STATUS_WX86_BREAKPOINT:
                exceptionContextType = ExceptionContextType::eSoftBreakpoint;
                break;
            case EXCEPTION_SINGLE_STEP:
            case STATUS_WX86_SINGLE_STEP:
                exceptionContextType = ExceptionContextType::eHardBreakpoint;
                break;
            default:
                exceptionContextType = ExceptionContextType::eException;
                break;
        }
    }

    [[nodiscard]] ExceptionContextType Type() const
    {
        return exceptionContextType;
    }

    [[nodiscard]] bool IsExternal() const
    {
        return isExternal;
    }
};

#endif //DEBUGGER_EXCEPTIONCONTEXT_HPP
