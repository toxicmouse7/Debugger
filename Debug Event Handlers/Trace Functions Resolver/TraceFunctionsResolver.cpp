#include <Zydis/Zydis.h>

#include "TraceFunctionsResolver.hpp"

void TraceFunctionsResolver::HandleDebugEvent(const ExceptionDebugEvent& event)
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

    BOOL isWow64 = false;
    auto process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, event.processId);
    IsWow64Process(process, &isWow64);

    ZydisDisassembledInstruction instruction;
    Utils::DisassembleInstruction(event.processId, context.Rip, isWow64, &instruction);

    try
    {
        if (instruction.info.mnemonic == ZYDIS_MNEMONIC_CALL)
        {
            ULONG_PTR functionAddress;
            if (instruction.operands[0].type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
            {
                functionAddress = instruction.operands[0].imm.value.u + instruction.runtime_address
                                  + instruction.info.length;
            }
            else if (instruction.operands[0].type == ZYDIS_OPERAND_TYPE_MEMORY)
            {
                functionAddress = ProcessMemoryManipulation::ReadPointer(event.processId,
                                                                         instruction.operands[0].mem.disp.value);
            }
            else if (instruction.operands[0].type == ZYDIS_OPERAND_TYPE_REGISTER)
            {
                functionAddress = Utils::GetValueFromRegister(context, instruction.operands[0].reg.value);
            }
            else
            {
                throw std::runtime_error("Unsupported operand type");
            }
            auto function = Utils::DisassembleFunction(event.processId, functionAddress, isWow64);
            std::list<uint64_t> arguments;
            std::ranges::for_each(*function.GetArgumentsStackOffset(), [&arguments, &context, &event](auto offset)
            {
                arguments.push_back(ProcessMemoryManipulation::ReadPointer(event.processId, context.Rsp + offset));
            });
            LogCall(instruction, functionAddress, arguments);
        }
        else if (instruction.info.mnemonic == ZYDIS_MNEMONIC_RET)
        {
            LogReturn(context.Rax, ProcessMemoryManipulation::ReadPointer(event.processId, context.Rsp));
        }
    }
    catch (const std::exception& e)
    {
        LogError(e.what());
    }

    context.Dr6 = 0;
    context.EFlags |= 0x100;

    if (!SetThreadContext(thread, &context))
    {
        throw std::runtime_error("Failed to set thread context");
    }

    CloseHandle(thread);
}

void TraceFunctionsResolver::LogException(DWORD pid, const std::string& exceptionName, DWORD exceptionCode,
                                          ULONG_PTR exceptionAddress) const
{

}

void TraceFunctionsResolver::LogCall(const ZydisDisassembledInstruction& instruction, ULONG_PTR functionAddress,
                                     const std::list<uint64_t>& arguments)
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << Utils::FormatAddress(instruction.runtime_address, "'") << '\t';
        ss << instruction.text << '\n';
        ss << "Function address: " << Utils::FormatAddress(functionAddress, "'") << '\n';
        if (!arguments.empty())
        {
            ss << "Arguments: ";
            std::ranges::for_each(arguments, [&ss](auto argument)
            {
                ss << Utils::FormatAddress(argument, "'") << ' ';
            });
            ss << '\n';
        }

        GetLogger().value().get().Log(ss.str());
    }
}

void TraceFunctionsResolver::LogReturn(ULONG_PTR returnValue, ULONG_PTR returnAddress)
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Return value: " << Utils::FormatAddress(returnValue, "'") << '\n';
        ss << "Return address: " << Utils::FormatAddress(returnAddress, "'") << '\n';

        GetLogger().value().get().Log(ss.str());
    }
}

void TraceFunctionsResolver::LogError(const std::string& errorMessage)
{
    if (GetLogger().has_value())
    {
        GetLogger().value().get().Log("Failed to trace function: " + errorMessage);
    }
}
