//
// Created by Aleksej on 08.12.2022.
//

#ifndef DEBUGGER_FUNCTION_HPP
#define DEBUGGER_FUNCTION_HPP

#include <vector>
#include <set>
#include <memory>
#include <map>
#include <list>
#include <Zydis/Zydis.h>

class Function
{
private:
    std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>> adjacencyList;
    std::shared_ptr<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>> instructions;
public:
    Function(const std::shared_ptr<std::map<uint64_t, std::set<uint64_t>>>& adjacencyList,
             const std::shared_ptr<std::map<uint64_t, std::vector<ZydisDisassembledInstruction>>>& instructions);

    std::shared_ptr<std::list<uint64_t>> GetAddressesOfReturn();
};


#endif //DEBUGGER_FUNCTION_HPP
