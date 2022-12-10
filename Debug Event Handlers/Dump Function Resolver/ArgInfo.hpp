//
// Created by Aleksej on 06.12.2022.
//

#ifndef DEBUGGER_ARGINFO_HPP
#define DEBUGGER_ARGINFO_HPP

#include <vector>
#include <string>

enum class ArgType
{
    number,
    pointer,
    stringPointer,
    wideStringPointer,
    structurePointer,
    bitField
};

struct ArgInfo
{
    uint32_t count = 0;
    std::vector<ArgType> types;
    std::vector<std::string> names;
    std::vector<std::string> values;
    std::vector<size_t> sizes;

    ArgInfo() = default;

    void AddArg(ArgType type, const std::string& name, const std::string& value, size_t size);

    static bool IsPointer(ArgType type);
};


#endif //DEBUGGER_ARGINFO_HPP
