//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_UTILS_HPP
#define DEBUGGER_UTILS_HPP

#include <Windows.h>
#include <list>
#include <string>
#include <optional>
#include <format>

#include "Debug Event Handlers/DebugEventHandlerType.hpp"

namespace Utils
{
    std::list<std::string> Split(const std::string& str, const std::string& delimiter);

    std::list<std::wstring> Split(const std::wstring& str, const std::wstring& delimiter);

    std::optional<std::string>
    FindLibraryByAddress(const std::list<std::pair<std::string, ULONG_PTR>>& libraries, ULONG_PTR address);

    std::optional<std::wstring>
    FindLibraryByAddress(const std::list<std::pair<std::wstring, ULONG_PTR>>& libraries, ULONG_PTR address);

    std::string FormatAddress(ULONG_PTR address, const std::string& delimiter);
    std::wstring FormatAddress(ULONG_PTR address, const std::wstring& delimiter);
};


#endif //DEBUGGER_UTILS_HPP
