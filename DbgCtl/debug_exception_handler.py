debug_exception_handler_path = '../Debug Event Handlers'

debug_exception_handler_header = """#ifndef DEBUGGER_{inc_name}_HPP
#define DEBUGGER_{inc_name}_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"

class {name} : public AbstractExceptionDebugEventHandler
{{
private:
    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

public:
    explicit {name}(std::optional<std::reference_wrapper<const Logger>> logger,
                    const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {{}}

    ~{name}() override = default;
}};

#endif //DEBUGGER_{inc_name}_HPP
"""

debug_exception_handler_cpp = """#include "{name}.hpp"

void {name}::HandleDebugEvent(const ExceptionDebugEvent& event)
{{
    
}}

void {name}::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                          ULONG_PTR exceptionAddress) const
{{

}}

"""
