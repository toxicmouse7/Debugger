//
// Created by Aleksej on 08.12.2022.
//

#include "Function.hpp"

Function::Function(const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList,
                   const std::shared_ptr<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>>& instructions,
                   const std::shared_ptr<std::set<uint64_t>>& argumentsStackOffset)
{
    this->adjacencyList = adjacencyList;
    this->instructions = instructions;
    this->argumentsStackOffset = argumentsStackOffset;
}

std::shared_ptr<std::list<uint64_t>> Function::GetAddressesOfReturn()
{
    auto addressesOfReturn = std::make_shared<std::list<uint64_t>>();

    for (auto& adjacency : *adjacencyList)
    {
        if (adjacency.second.empty())
            addressesOfReturn->push_back((*instructions)[adjacency.first].back().runtime_address);
    }

    return addressesOfReturn;
}

std::shared_ptr<const std::set<uint64_t>> Function::GetArgumentsStackOffset() const
{
    return argumentsStackOffset;
}
