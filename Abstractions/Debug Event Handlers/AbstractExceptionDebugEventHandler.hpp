//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEXCEPTIONDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTEXCEPTIONDEBUGEVENTHANDLER_HPP

#include <list>
#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/ExceptionDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractExceptionDebugEventHandler : public AbstractEventRecipient
{
private:
    //std::list<>
    std::optional<std::reference_wrapper<const Logger>> logger;

    void SetLogger(std::reference_wrapper<const Logger> loggerReference)
    {
        this->logger = loggerReference;
    }

protected:
    [[nodiscard]] virtual const std::optional<std::reference_wrapper<const Logger>>& GetLogger() const
    {
        return logger;
    }

    virtual void HandleDebugEvent(const ExceptionDebugEvent& event) = 0;

    virtual void HandleBreakpoint(const ExceptionDebugEvent& event) = 0;

    virtual void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                              ULONG_PTR exceptionAddress) const = 0;

    virtual void LogBreakpoint() const = 0;

    static std::string ExceptionCodeToString(DWORD exceptionCode)
    {
        switch (exceptionCode)
        {
            case EXCEPTION_ACCESS_VIOLATION:
                return "The thread tried to read from or write to a virtual address for which"
                       " it does not have the appropriate access.";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                return "The thread tried to access an array element that is out of bounds and the underlying hardware"
                       " supports bounds checking.";
            case EXCEPTION_BREAKPOINT:
                return "A breakpoint was encountered.";
            case EXCEPTION_DATATYPE_MISALIGNMENT:
                return "The thread tried to read or write data that is misaligned on"
                       " hardware that does not provide alignment.";
            case EXCEPTION_FLT_DENORMAL_OPERAND:
                return "One of the operands in a floating-point operation is denormal."
                       " A denormal value is one that is too small to represent as a standard floating-point value.";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                return "The thread tried to divide a floating-point value by a floating-point divisor of zero.";
            case EXCEPTION_FLT_INEXACT_RESULT:
                return "The result of a floating-point operation cannot be represented exactly as a decimal fraction.";
            case EXCEPTION_FLT_INVALID_OPERATION:
                return "This exception represents any floating-point exception not included in this list.";
            case EXCEPTION_FLT_OVERFLOW:
                return "The exponent of a floating-point operation is greater than"
                       " the magnitude allowed by the corresponding type.";
            case EXCEPTION_FLT_STACK_CHECK:
                return "The stack overflowed or underflowed as the result of a floating-point operation.";
            case EXCEPTION_FLT_UNDERFLOW:
                return "The exponent of a floating-point operation is less than"
                       " the magnitude allowed by the corresponding type.";
            case EXCEPTION_ILLEGAL_INSTRUCTION:
                return "The thread tried to execute an invalid instruction.";
            case EXCEPTION_IN_PAGE_ERROR:
                return "The thread tried to access a page that was not present,"
                       " and the system was unable to load the page.";
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                return "The thread tried to divide an integer value by an integer divisor of zero.";
            case EXCEPTION_INT_OVERFLOW:
                return "The result of an integer operation caused a carry out of"
                       " the most significant bit of the result.";
            case EXCEPTION_INVALID_DISPOSITION:
                return "An exception handler returned an invalid disposition to the exception dispatcher.";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                return "The thread tried to continue execution after a noncontinuable exception occurred.";
            case EXCEPTION_PRIV_INSTRUCTION:
                return "The thread tried to execute an instruction whose"
                       " operation is not allowed in the current machine mode.";
            case EXCEPTION_SINGLE_STEP:
                return "A trace trap or other single-instruction mechanism"
                       " signaled that one instruction has been executed.";
            case EXCEPTION_STACK_OVERFLOW:
                return "The thread used up its stack.";
            default:
                return "Unknown exception";
        }
    }

public:
    explicit AbstractExceptionDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const ExceptionDebugEvent&>(event));
    }

    ~AbstractExceptionDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTEXCEPTIONDEBUGEVENTHANDLER_HPP
