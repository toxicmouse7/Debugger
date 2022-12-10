//
// Created by Aleksej on 06.12.2022.
//

#include "ArgInfo.hpp"

void ArgInfo::AddArg(ArgType type, const std::string& name, const std::string& value, size_t size)
{
    count++;
    types.push_back(type);
    names.push_back(name);
    values.push_back(value);
    sizes.push_back(size);
}

bool ArgInfo::IsPointer(ArgType type)
{
    return type == ArgType::pointer || type == ArgType::stringPointer || type == ArgType::wideStringPointer ||
           type == ArgType::structurePointer;
}
