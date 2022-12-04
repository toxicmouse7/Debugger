//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEXITPROCESSDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTEXITPROCESSDEBUGEVENTHANDLER_HPP

#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/ExitProcessDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractExitProcessDebugEventHandler : public AbstractEventRecipient
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

    virtual void HandleDebugEvent(const ExitProcessDebugEvent& event) = 0;

    virtual void LogExitProcess(DWORD pid, DWORD exitCode) const = 0;

public:
    explicit AbstractExitProcessDebugEventHandler(std::optional<std::reference_wrapper<const Logger>> logger)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const ExitProcessDebugEvent&>(event));
    }

    ~AbstractExitProcessDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTEXITPROCESSDEBUGEVENTHANDLER_HPP
