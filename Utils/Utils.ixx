//
// Created by Aleksej on 02.12.2022.
//

export module Utils;

import std.core;

export namespace Utils
{
    std::list<std::string> Split(const std::string& str, const std::string& delimiter);
    std::list<std::wstring> Split(const std::wstring& str, const std::wstring& delimiter);
}

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