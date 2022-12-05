//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_UTILS_HPP
#define DEBUGGER_UTILS_HPP

#include <Windows.h>
#include <list>
#include <sstream>
#include <optional>
#include <format>
#include <Zydis/Zydis.h>

#include "Debug Event Handlers/DebugEventHandlerType.hpp"
#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"

class Utils
{
public:
    static std::list<std::string> Split(const std::string& str, const std::string& delimiter);

    static std::list<std::wstring> Split(const std::wstring& str, const std::wstring& delimiter);

    static std::optional<std::string>
    FindLibraryByAddress(const std::list<std::pair<std::string, ULONG_PTR>>& libraries, ULONG_PTR address);

    static std::optional<std::wstring>
    FindLibraryByAddress(const std::list<std::pair<std::wstring, ULONG_PTR>>& libraries, ULONG_PTR address);

    static std::string FormatAddress(ULONG_PTR address, const std::string& delimiter);

    static std::wstring FormatAddress(ULONG_PTR address, const std::wstring& delimiter);

    static void DisassembleInstruction(DWORD processId, const CONTEXT& context, bool isWow64,
                                       ZydisDisassembledInstruction* instruction);

    static std::string FormatRegisters(const CONTEXT& context, bool isWow64);
};


#endif //DEBUGGER_UTILS_HPP
