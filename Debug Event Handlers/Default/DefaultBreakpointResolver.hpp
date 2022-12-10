//
// Created by Aleksej on 06.12.2022.
//

#ifndef DEBUGGER_DEFAULTBREAKPOINTRESOLVER_HPP
#define DEBUGGER_DEFAULTBREAKPOINTRESOLVER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"

class DefaultBreakpointResolver : public AbstractExceptionDebugEventHandler
{
private:
    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

public:
    explicit DefaultBreakpointResolver(std::optional<std::reference_wrapper<const Logger>> logger,
                                       const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {}

    ~DefaultBreakpointResolver() override = default;
};


#endif //DEBUGGER_DEFAULTBREAKPOINTRESOLVER_HPP
