//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"

class DefaultExceptionDebugEventHandler : public AbstractExceptionDebugEventHandler
{
private:
    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

public:
    explicit DefaultExceptionDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger,
                                               const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {}

    ~DefaultExceptionDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTEXCEPTIONDEBUGEVENTHANDLER_HPP
