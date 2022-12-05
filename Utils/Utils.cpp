//
// Created by Aleksej on 02.12.2022.
//

#include "Utils.hpp"

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

std::optional<std::string>
Utils::FindLibraryByAddress(const std::list<std::pair<std::string, ULONG_PTR>>& libraries, ULONG_PTR address)
{
    auto it = std::find_if(libraries.begin(), libraries.end(),
                           [address](const auto& pair)
                           {
                               return pair.second == address;
                           });

    if (it == libraries.end())
        return std::nullopt;

    return it->first;
}

std::optional<std::wstring>
Utils::FindLibraryByAddress(const std::list<std::pair<std::wstring, ULONG_PTR>>& libraries, ULONG_PTR address)
{
    auto it = std::find_if(libraries.begin(), libraries.end(),
                           [address](const auto& pair)
                           {
                               return pair.second == address;
                           });

    if (it == libraries.end())
        return std::nullopt;

    return it->first;
}

std::string Utils::FormatAddress(ULONG_PTR address, const std::string& delimiter)
{
    return std::format("{:0>8X}", address >> 32) + delimiter + std::format("{:0>8X}", address & 0xFFFFFFFF);
}

std::wstring Utils::FormatAddress(ULONG_PTR address, const std::wstring& delimiter)
{
    return std::format(L"{:0>8X}", address >> 32) + delimiter + std::format(L"{:0>8X}", address & 0xFFFFFFFF);
}

void Utils::DisassembleInstruction(DWORD processId, const CONTEXT& context, bool isWow64,
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

    ProcessMemoryManipulation::ReadBytes(processId, context.Rip, buffer, 15);

    ZydisDisassembleIntel(machineMode, context.Rip, buffer, 15, instruction);
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
