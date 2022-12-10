//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_TRACERESOLVER_HPP
#define DEBUGGER_TRACERESOLVER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"
#include "Zydis/Disassembler.h"

class TraceResolver : public AbstractExceptionDebugEventHandler
{
private:
    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

    void LogInstruction(ULONG_PTR instructionAddress, const ZydisDisassembledInstruction& instruction,
                        const CONTEXT& context, bool isWow64);

public:
    explicit TraceResolver(std::optional<std::reference_wrapper<const Logger>> logger,
                           const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {}

    ~TraceResolver() override = default;
};


#endif //DEBUGGER_TRACERESOLVER_HPP
