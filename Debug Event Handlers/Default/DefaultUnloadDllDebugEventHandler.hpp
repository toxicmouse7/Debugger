//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_DEFAULTUNLOADDLLDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTUNLOADDLLDEBUGEVENTHANDLER_HPP

#include "Abstractions/Debug Event Handlers/AbstractUnloadDllDebugEventHandler.hpp"

class DefaultUnloadDllDebugEventHandler : public AbstractUnloadDllDebugEventHandler
{
private:
    void HandleDebugEvent(const UnloadDllDebugEvent& event) override;

    void LogUnloadDll(const std::wstring& libraryName, ULONG_PTR baseAddress) const override;

public:
    explicit DefaultUnloadDllDebugEventHandler(std::map<std::wstring, ULONG_PTR>& libraries,
                                               std::optional<std::reference_wrapper<const Logger>> logger)
            : AbstractUnloadDllDebugEventHandler(libraries, logger) {}

    ~DefaultUnloadDllDebugEventHandler() override = default;
};


#endif //DEBUGGER_DEFAULTUNLOADDLLDEBUGEVENTHANDLER_HPP
