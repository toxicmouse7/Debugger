//
// Created by Aleksej on 07.12.2022.
//

#ifndef DEBUGGER_BITFIELDSDATABASE_HPP
#define DEBUGGER_BITFIELDSDATABASE_HPP

#include <map>
#include <string>

#include "ArgInfo.hpp"

class BitfieldsDatabase
{
private:
    inline static BitfieldsDatabase* instance = nullptr;
    std::map<std::string, ArgInfo> bitfields;

public:
    static BitfieldsDatabase* GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new BitfieldsDatabase();
        }

        return instance;
    }

    void AddBitfield(const std::string& name, ArgInfo args)
    {
        bitfields[name] = std::move(args);
    }

    [[nodiscard]] const ArgInfo& GetBitfield(const std::string& name) const
    {
        return bitfields.at(name);
    }
};


#endif //DEBUGGER_BITFIELDSDATABASE_HPP
