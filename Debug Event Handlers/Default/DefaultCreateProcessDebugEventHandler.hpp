//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractCreateProcessDebugEventHandler.hpp"

class DefaultCreateProcessDebugEventHandler : public AbstractCreateProcessDebugEventHandler
{
private:
    void HandleDebugEvent(const CreateProcessDebugEvent& event) override;

    void LogCreateProcess(DWORD pid, DWORD tid, const std::string& processName,
                          ULONG_PTR baseAddress, ULONG_PTR entryPoint) const override;

    void LogCreateProcess(DWORD pid, DWORD tid, const std::wstring& processName,
                          ULONG_PTR baseAddress, ULONG_PTR entryPoint) const override;

public:

    explicit DefaultCreateProcessDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger);

    ~DefaultCreateProcessDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTCREATEPROCESSDEBUGEVENTHANDLER_HPP
