//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTCREATEPROCESSDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTCREATEPROCESSDEBUGEVENTHANDLER_HPP

#include <list>
#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/CreateProcessDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"
#include "Debugger/Breakpoint.hpp"

class AbstractCreateProcessDebugEventHandler : public AbstractEventRecipient
{
private:
    std::optional<std::reference_wrapper<const Logger>> logger;

    void SetLogger(std::reference_wrapper<const Logger> loggerReference)
    {
        this->logger = loggerReference;
    }

protected:
    [[nodiscard]] virtual const std::optional<std::reference_wrapper<const Logger>>& GetLogger() const
    {
        return logger;
    }

    virtual void HandleDebugEvent(const CreateProcessDebugEvent& event) = 0;

    virtual void LogCreateProcess(DWORD pid, DWORD tid, const std::string& processName,
                                  ULONG_PTR baseAddress, ULONG_PTR entryPoint) const = 0;

    virtual void LogCreateProcess(DWORD pid, DWORD tid, const std::wstring& libraryName,
                                  ULONG_PTR baseAddress, ULONG_PTR entryPoint) const = 0;

public:
    explicit AbstractCreateProcessDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const CreateProcessDebugEvent&>(event));
    }

    ~AbstractCreateProcessDebugEventHandler() override = default;
};

#endif //DEBUGGER_ABSTRACTCREATEPROCESSDEBUGEVENTHANDLER_HPP
