//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractLoadDllDebugEventHandler.hpp"
#include "Process Memory Manipulation/ProcessMemoryManipulation.hpp"

class DefaultLoadDllDebugEventHandler : public AbstractLoadDllDebugEventHandler
{
private:
    void HandleDebugEvent(const LoadDllDebugEvent& event) override;

    void LogLibraryLoad(const std::wstring& libraryName, ULONG_PTR baseAddress) const override;

public:

    DefaultLoadDllDebugEventHandler(std::map<std::wstring, ULONG_PTR>& libraries,
                                    std::optional<std::reference_wrapper<const Logger>> logger);

    ~DefaultLoadDllDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP
