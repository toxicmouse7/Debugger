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
#include <memory>
#include <stack>
#include <set>
#include <Zydis/Zydis.h>

#include "Debug Event Handlers/DebugEventHandlerType.hpp"
#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"
#include "Debugger/Function.hpp"

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

    static void DisassembleInstruction(DWORD processId, ULONG_PTR address, bool isWow64,
                                       ZydisDisassembledInstruction* instruction);

    static Function DisassembleFunction(DWORD processId,
                                        ULONG_PTR address,
                                        bool isWow64);

    static std::string FormatRegisters(const CONTEXT& context, bool isWow64);

    static bool IsWow64(DWORD processId);

    static ULONG_PTR GetValueFromRegister(CONTEXT context, ZydisRegister aRegister);

private:
    static bool IsConditionalJumpInstruction(const ZydisDisassembledInstruction& instruction);

    static bool IsJumpInstruction(const ZydisDisassembledInstruction& instruction);

    static std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>
    GetAdjacencyList(DWORD processId, ULONG_PTR address, bool isWow64);

    static std::shared_ptr<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>>
    GetInstructionsFromAdjacencyList(DWORD processId, bool isWow64,
                                     const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList);

    static ULONG_PTR GetJumpOffsetFromInstruction(DWORD processId, const ZydisDisassembledInstruction& instruction);

    static std::shared_ptr<std::set<uint64_t>>
    GetArgumentsFromAdjacencyList(DWORD processId, bool isWow64,
                                  const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList);
};


#endif //DEBUGGER_UTILS_HPP
