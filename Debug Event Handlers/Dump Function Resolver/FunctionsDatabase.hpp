//
// Created by Aleksej on 06.12.2022.
//

#ifndef DEBUGGER_FUNCTIONSDATABASE_HPP
#define DEBUGGER_FUNCTIONSDATABASE_HPP

#include <map>
#include <string>
#include <Windows.h>

#include "ArgInfo.hpp"

class FunctionsDatabase
{
private:
    inline static FunctionsDatabase* instance = nullptr;
    std::map<ULONG_PTR, std::string> functions;
    std::map<std::string, ArgInfo> functionsArgs;

    FunctionsDatabase() = default;

public:

    static FunctionsDatabase* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new FunctionsDatabase();
        }

        return instance;
    }

    void AddFunction(const std::string& name, ULONG_PTR address, ArgInfo args)
    {
        functions[address] = name;
        functionsArgs[name] = std::move(args);
    }

    [[nodiscard]] const std::string& GetFunctionName(ULONG_PTR address) const
    {
        return functions.at(address);
    }

    [[nodiscard]] const ArgInfo& GetFunctionArgs(const std::string& name) const
    {
        return functionsArgs.at(name);
    }
};


#endif //DEBUGGER_FUNCTIONSDATABASE_HPP
