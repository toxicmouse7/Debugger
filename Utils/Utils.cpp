//
// Created by Aleksej on 02.12.2022.
//

#include "Utils.hpp"

#include <memory>

std::list<std::string> Utils::Split(const std::string& str, const std::string& delimiter)
{
    std::list<std::string> result;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;

    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        result.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    result.push_back(str.substr(prev));

    return result;
}

std::list<std::wstring> Utils::Split(const std::wstring& str, const std::wstring& delimiter)
{
    std::list<std::wstring> result;
    std::wstring::size_type pos = 0;
    std::wstring::size_type prev = 0;

    while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
    {
        result.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    result.push_back(str.substr(prev));

    return result;
}

std::string Utils::FormatAddress(ULONG_PTR address, const std::string& delimiter)
{
    return std::format("{:0>8X}", address >> 32) + delimiter + std::format("{:0>8X}", address & 0xFFFFFFFF);
}

std::wstring Utils::FormatAddress(ULONG_PTR address, const std::wstring& delimiter)
{
    return std::format(L"{:0>8X}", address >> 32) + delimiter + std::format(L"{:0>8X}", address & 0xFFFFFFFF);
}

void Utils::DisassembleInstruction(DWORD processId, ULONG_PTR address, bool isWow64,
                                   ZydisDisassembledInstruction* instruction)
{
    BYTE buffer[15];
    ZydisMachineMode machineMode;

    if (isWow64)
    {
        machineMode = ZYDIS_MACHINE_MODE_LONG_COMPAT_32;
    }
    else
    {
        machineMode = ZYDIS_MACHINE_MODE_LONG_64;
    }

    ProcessMemoryManipulation::ReadBytes(processId, address, buffer, 15);

    ZydisDisassembleIntel(machineMode, address, buffer, 15, instruction);
}

std::string Utils::FormatRegisters(const CONTEXT& context, bool isWow64)
{
    std::stringstream ss;
    if (isWow64)
    {
        ss << std::format("EAX: {:0>8X} EBX: {:0>8X} ECX: {:0>8X} EDX: {:0>8X}", context.Rax, context.Rbx,
                          context.Rcx, context.Rdx) << '\n';
        ss << std::format("ESI: {:0>8X} EDI: {:0>8X} EBP: {:0>8X} ESP: {:0>8X}", context.Rsi, context.Rdi,
                          context.Rbp, context.Rsp) << '\n';
        ss << std::format("E8:  {:0>8X} E9:  {:0>8X} E10: {:0>8X} E11: {:0>8X}", context.R8, context.R9,
                          context.R10, context.R11) << '\n';
        ss << std::format("E12: {:0>8X} E13: {:0>8X} E14: {:0>8X} E15: {:0>8X}", context.R12, context.R13,
                          context.R14, context.R15) << '\n';
        ss << std::format("RIP: {:0>8X} EFLAGS: {:0>8X}", context.Rip, context.EFlags) << '\n';
    }
    else
    {
        ss << std::format("RAX: {:0>16X} RBX: {:0>16X} RCX: {:0>16X} RDX: {:0>16X}", context.Rax, context.Rbx,
                          context.Rcx, context.Rdx) << '\n';
        ss << std::format("RSI: {:0>16X} RDI: {:0>16X} RBP: {:0>16X} RSP: {:0>16X}", context.Rsi, context.Rdi,
                          context.Rbp, context.Rsp) << '\n';
        ss << std::format("R8:  {:0>16X} R9:  {:0>16X} R10: {:0>16X} R11: {:0>16X}", context.R8, context.R9,
                          context.R10, context.R11) << '\n';
        ss << std::format("R12: {:0>16X} R13: {:0>16X} R14: {:0>16X} R15: {:0>16X}", context.R12, context.R13,
                          context.R14, context.R15) << '\n';
        ss << std::format("RIP: {:0>16X} EFLAGS: {:0>16X}", context.Rip, context.EFlags) << '\n';
    }

    return ss.str();
}

Function Utils::DisassembleFunction(DWORD processId, ULONG_PTR address, bool isWow64)
{
    auto adjacencyList = GetAdjacencyList(processId, address, isWow64);

    auto instructions = GetInstructionsFromAdjacencyList(processId, isWow64, adjacencyList);

    auto argumentsStackOffset = GetArgumentsFromAdjacencyList(processId, isWow64, adjacencyList);

    return {adjacencyList, instructions, argumentsStackOffset};
}

std::shared_ptr<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>>
Utils::GetInstructionsFromAdjacencyList(DWORD processId, bool isWow64,
                                        const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList)
{
    auto instructions = std::make_shared<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>>();

    for (auto& adjacencyEntry: *adjacencyList)
    {
        auto pieceEntry = adjacencyEntry.first;
        while (true)
        {
            ZydisDisassembledInstruction instruction;
            DisassembleInstruction(processId, pieceEntry, isWow64, &instruction);

            (*instructions)[adjacencyEntry.first].push_back(instruction);
            pieceEntry += instruction.info.length;

            if (IsConditionalJumpInstruction(instruction) || IsJumpInstruction(instruction))
            {
                adjacencyEntry.second.insert(Utils::GetJumpOffsetFromInstruction(processId, instruction));
                if (IsJumpInstruction(instruction)) break;
            }

            if (adjacencyList->contains(pieceEntry))
            {
                adjacencyEntry.second.insert(pieceEntry);
                break;
            }

            if (instruction.info.mnemonic == ZYDIS_MNEMONIC_RET)
            {
                break;
            }
        }
    }

    return instructions;
}

std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>> Utils::GetAdjacencyList(DWORD processId, ULONG_PTR address,
                                                                                bool isWow64)
{
    auto adjacency = std::make_shared<std::map<uint64_t, std::set<uint64_t>>>();
    std::stack<uint64_t> toVisit;
    toVisit.push(address);

    (*adjacency)[address] = {};

    while (!toVisit.empty())
    {
        auto currentAddress = toVisit.top();
        toVisit.pop();
        ZydisDisassembledInstruction instruction;
        DisassembleInstruction(processId, currentAddress, isWow64, &instruction);

        if (IsConditionalJumpInstruction(instruction))
        {
            auto newAddress = currentAddress + instruction.info.length;
            auto newConditionalAddress = Utils::GetJumpOffsetFromInstruction(processId, instruction);

            if (!adjacency->contains(newConditionalAddress))
            {
                (*adjacency)[newConditionalAddress] = {};
                toVisit.push(newConditionalAddress);
            }
            if (!adjacency->contains(newAddress))
            {
                (*adjacency)[newAddress] = {};
                toVisit.push(newAddress);
            }
        }
        else if (IsJumpInstruction(instruction))
        {
            auto newAddress = Utils::GetJumpOffsetFromInstruction(processId, instruction);
            if (!adjacency->contains(newAddress))
            {
                (*adjacency)[newAddress] = {};
                toVisit.push(newAddress);
            }
        }
        else if (instruction.info.mnemonic != ZYDIS_MNEMONIC_RET)
        {
            auto newAddress = currentAddress + instruction.info.length;
            toVisit.push(newAddress);
        }
    }
    return adjacency;
}

bool Utils::IsConditionalJumpInstruction(const ZydisDisassembledInstruction& instruction)
{
    switch (instruction.info.mnemonic)
    {
        case ZYDIS_MNEMONIC_JZ:
        case ZYDIS_MNEMONIC_JB:
        case ZYDIS_MNEMONIC_JBE:
        case ZYDIS_MNEMONIC_JL:
        case ZYDIS_MNEMONIC_JLE:
        case ZYDIS_MNEMONIC_JNB:
        case ZYDIS_MNEMONIC_JNBE:
        case ZYDIS_MNEMONIC_JNL:
        case ZYDIS_MNEMONIC_JNLE:
        case ZYDIS_MNEMONIC_JNO:
        case ZYDIS_MNEMONIC_JNP:
        case ZYDIS_MNEMONIC_JNS:
        case ZYDIS_MNEMONIC_JNZ:
        case ZYDIS_MNEMONIC_JO:
        case ZYDIS_MNEMONIC_JP:
        case ZYDIS_MNEMONIC_JS:
            return true;
        default:
            return false;
    }
}

bool Utils::IsJumpInstruction(const ZydisDisassembledInstruction& instruction)
{
    return instruction.info.mnemonic == ZYDIS_MNEMONIC_JMP;
}

ULONG_PTR Utils::GetJumpOffsetFromInstruction(DWORD processId, const ZydisDisassembledInstruction& instruction)
{
    if (!IsJumpInstruction(instruction) && !IsConditionalJumpInstruction(instruction))
        throw std::runtime_error("Instruction is not a jump instruction");

    switch (instruction.operands[0].type)
    {
        case ZYDIS_OPERAND_TYPE_IMMEDIATE:
        {
            return instruction.runtime_address + instruction.info.length + instruction.operands[0].imm.value.s;
        }
        case ZYDIS_OPERAND_TYPE_MEMORY:
        {
            return ProcessMemoryManipulation::ReadPointer(processId, instruction.operands[0].mem.disp.value);
        }
        default:
            throw std::runtime_error("Unknown jump operand type");
    }
}

bool Utils::IsWow64(DWORD processId)
{
    auto process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    BOOL isWow64;
    if (!IsWow64Process(process, &isWow64))
    {
        throw std::runtime_error("Failed to check if process is 32-bit (" + std::to_string(GetLastError()) + ")");
    }
    CloseHandle(process);
    return isWow64;
}

ULONG_PTR Utils::GetValueFromRegister(CONTEXT context, ZydisRegister aRegister)
{
    switch (aRegister)
    {
        case ZYDIS_REGISTER_RAX:
        case ZYDIS_REGISTER_EAX:
            return context.Rax;
        case ZYDIS_REGISTER_RCX:
        case ZYDIS_REGISTER_ECX:
            return context.Rcx;
        case ZYDIS_REGISTER_RDX:
        case ZYDIS_REGISTER_EDX:
            return context.Rdx;
        case ZYDIS_REGISTER_RBX:
        case ZYDIS_REGISTER_EBX:
            return context.Rbx;
        case ZYDIS_REGISTER_RSP:
        case ZYDIS_REGISTER_ESP:
            return context.Rsp;
        case ZYDIS_REGISTER_RBP:
        case ZYDIS_REGISTER_EBP:
            return context.Rbp;
        case ZYDIS_REGISTER_RSI:
        case ZYDIS_REGISTER_ESI:
            return context.Rsi;
        case ZYDIS_REGISTER_RDI:
        case ZYDIS_REGISTER_EDI:
            return context.Rdi;
        case ZYDIS_REGISTER_R8:
        case ZYDIS_REGISTER_R8D:
            return context.R8;
        case ZYDIS_REGISTER_R9:
        case ZYDIS_REGISTER_R9D:
            return context.R9;
        case ZYDIS_REGISTER_R10:
        case ZYDIS_REGISTER_R10D:
            return context.R10;
        case ZYDIS_REGISTER_R11:
        case ZYDIS_REGISTER_R11D:
            return context.R11;
        case ZYDIS_REGISTER_R12:
        case ZYDIS_REGISTER_R12D:
            return context.R12;
        case ZYDIS_REGISTER_R13:
        case ZYDIS_REGISTER_R13D:
            return context.R13;
        case ZYDIS_REGISTER_R14:
        case ZYDIS_REGISTER_R14D:
            return context.R14;
        case ZYDIS_REGISTER_R15:
        case ZYDIS_REGISTER_R15D:
            return context.R15;
        case ZYDIS_REGISTER_RIP:
        case ZYDIS_REGISTER_EIP:
            return context.Rip;
        case ZYDIS_REGISTER_RFLAGS:
        case ZYDIS_REGISTER_EFLAGS:
            return context.EFlags;
        case ZYDIS_REGISTER_CS:
            return context.SegCs;
        case ZYDIS_REGISTER_SS:
            return context.SegSs;
        case ZYDIS_REGISTER_DS:
            return context.SegDs;
        case ZYDIS_REGISTER_ES:
            return context.SegEs;
        case ZYDIS_REGISTER_FS:
            return context.SegFs;
        case ZYDIS_REGISTER_GS:
            return context.SegGs;
        default:
            throw std::runtime_error("Unknown register " + std::to_string(aRegister));
    }
}

std::shared_ptr<std::set<uint64_t>>
Utils::GetArgumentsFromAdjacencyList(DWORD processId, bool isWow64,
                                     const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList)
{
    auto arguments = std::make_shared<std::set<uint64_t>>();

    auto checkIfArgumentAccessed = [](const ZydisDecodedOperand& operand)
    {
        if (operand.type == ZYDIS_OPERAND_TYPE_MEMORY)
        {
            if (operand.mem.base == ZYDIS_REGISTER_RBP || operand.mem.base == ZYDIS_REGISTER_EBP ||
                operand.mem.base == ZYDIS_REGISTER_RSP || operand.mem.base == ZYDIS_REGISTER_ESP)
            {
                if (operand.mem.disp.value > 0) return true;
            }
        }

        return false;
    };

    for (auto& adjacencyEntry: *adjacencyList)
    {
        auto pieceEntry = adjacencyEntry.first;
        while (true)
        {
            ZydisDisassembledInstruction instruction;
            DisassembleInstruction(processId, pieceEntry, isWow64, &instruction);

            if (checkIfArgumentAccessed(instruction.operands[0]))
            {
                arguments->insert(instruction.operands[0].mem.disp.value);
            }
            else if (checkIfArgumentAccessed(instruction.operands[1]))
            {
                arguments->insert(instruction.operands[1].mem.disp.value);
            }

            pieceEntry += instruction.info.length;

            if (IsJumpInstruction(instruction) || IsConditionalJumpInstruction(instruction) ||
                adjacencyList->contains(pieceEntry) ||
                instruction.info.mnemonic == ZYDIS_MNEMONIC_RET)
            {
                break;
            }
        }
    }

    return arguments;
}
