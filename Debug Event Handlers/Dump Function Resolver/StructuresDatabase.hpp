//
// Created by Aleksej on 07.12.2022.
//

#ifndef DEBUGGER_STRUCTURESDATABASE_HPP
#define DEBUGGER_STRUCTURESDATABASE_HPP

#include <map>
#include <string>

#include "ArgInfo.hpp"

class StructuresDatabase
{
private:
    inline static StructuresDatabase* instance = nullptr;
    std::map<std::string, ArgInfo> structures;

public:
    static StructuresDatabase* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new StructuresDatabase();
        }

        return instance;
    }

    void AddStructure(const std::string& name, ArgInfo args)
    {
        structures[name] = std::move(args);
    }

    [[nodiscard]] const ArgInfo& GetStructure(const std::string& name) const
    {
        return structures.at(name);
    }
};


#endif //DEBUGGER_STRUCTURESDATABASE_HPP
