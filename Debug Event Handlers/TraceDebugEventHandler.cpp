//
// Created by Aleksej on 04.12.2022.
//

#include "TraceDebugEventHandler.hpp"
#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"
#include "Utils/Utils.hpp"

#include <Zydis/Zydis.h>

void TraceDebugEventHandler::HandleDebugEvent(const ExceptionDebugEvent& event)
{
    auto thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT, FALSE, event.threadId);
    if (thread == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to open thread");
    }

    CONTEXT context;
    context.ContextFlags = CONTEXT_ALL;
    if (!GetThreadContext(thread, &context))
    {
        throw std::runtime_error("Failed to get thread context");
    }

    if (exceptionContext.Type() == ExceptionContextType::eSoftBreakpoint)
        context.Rip -= 1;

    BOOL isWow64 = false;
    auto process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, event.processId);
    IsWow64Process(process, &isWow64);

    ZydisDisassembledInstruction instruction;
    Utils::DisassembleInstruction(event.processId, context, isWow64, &instruction);

    if (instruction.info.mnemonic == ZYDIS_MNEMONIC_JMP || instruction.info.mnemonic == ZYDIS_MNEMONIC_CALL)
    {
        LogInstruction(context.Rip, instruction, context, isWow64);
    }

    context.Dr6 = 0;
    context.EFlags |= 0x100;

    if (!SetThreadContext(thread, &context))
    {
        throw std::runtime_error("Failed to set thread context");
    }

    CloseHandle(thread);
}

void TraceDebugEventHandler::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                          ULONG_PTR exceptionAddress) const
{

}

void TraceDebugEventHandler::LogInstruction(ULONG_PTR instructionAddress,
                                            const ZydisDisassembledInstruction& instruction,
                                            const CONTEXT& context, bool isWow64)
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << Utils::FormatAddress(instructionAddress, "'") << '\t';
        ss << instruction.text << '\n';
        ss << Utils::FormatRegisters(context, isWow64);

        GetLogger().value().get().Log(ss.str());
    }
}
