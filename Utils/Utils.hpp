//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_UTILS_HPP
#define DEBUGGER_UTILS_HPP

#include <list>
#include <string>

namespace Utils
{
    std::list<std::string> Split(const std::string& str, const std::string& delimiter);
    std::list<std::wstring> Split(const std::wstring& str, const std::wstring& delimiter);
};


#endif //DEBUGGER_UTILS_HPP
