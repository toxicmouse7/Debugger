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
