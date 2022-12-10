//
// Created by Aleksej on 06.12.2022.
//

#ifndef DEBUGGER_DUMPFUNCTION_HPP
#define DEBUGGER_DUMPFUNCTION_HPP

#include "Abstractions/Debug Event Handlers/AbstractExceptionDebugEventHandler.hpp"
#include "FunctionsDatabase.hpp"
#include "StructuresDatabase.hpp"
#include "BitfieldsDatabase.hpp"

class DumpFunction : public AbstractExceptionDebugEventHandler
{
private:
    inline static ULONG_PTR lastAddress = 0;
    inline static std::shared_ptr<std::list<ULONG_PTR>> retBreakpoints = nullptr;

    void HandleDebugEvent(const ExceptionDebugEvent& event) override;

    void LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                      ULONG_PTR exceptionAddress) const override;

    void LogFunctionCall(DWORD pid, ULONG_PTR functionAddress, ULONG_PTR callAddress, ULONG_PTR stackAddress,
                         bool isWow64) const;

    static std::wstring ParseArgs(DWORD pid, const ArgInfo& argInfo, bool isWow64, ULONG_PTR stackAddress);

    static std::wstring ParseStructure(DWORD pid, std::string structureName, bool isWow64, ULONG_PTR structureAddress,
                                       uint32_t nestingLevel);

    static std::wstring ParseBitfield(const std::string& bitfieldName, ULONG_PTR bitfieldValue, uint32_t nestingLevel);

    static void AddNesting(std::wstringstream& ss, uint32_t nesting);

    void LogFunctionReturn(DWORD pid, ULONG_PTR functionAddress, ULONG_PTR stackAddress, bool isWow64,
                           ULONG_PTR returnValue) const;

public:
    explicit DumpFunction(std::optional<std::reference_wrapper<const Logger>> logger,
                          const ExceptionContext& exceptionContext)
            : AbstractExceptionDebugEventHandler(logger, exceptionContext) {}

    ~DumpFunction() override = default;
};


#endif //DEBUGGER_DUMPFUNCTION_HPP
