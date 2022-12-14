#ifndef DEBUGGER_TRACEFUNCTIONSRESOLVER_HPP
#define DEBUGGER_TRACEFUNCTIONSRESOLVER_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"

class TraceFunctionsResolver : public AbstractExceptionDebugEventHandler
{
private:
    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

    void LogCall(const ZydisDisassembledInstruction& instruction, ULONG_PTR functionAddress,
                 const std::list<uint64_t>& arguments);

    void LogReturn(ULONG_PTR returnValue, ULONG_PTR returnAddress);

    void LogError(const std::string& errorMessage);

public:
    explicit TraceFunctionsResolver(std::optional<std::reference_wrapper<const Logger>> logger,
                    const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {}

    ~TraceFunctionsResolver() override = default;
};

#endif //DEBUGGER_TRACEFUNCTIONSRESOLVER_HPP
