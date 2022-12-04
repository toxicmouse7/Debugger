//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTCREATETHREADDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTCREATETHREADDEBUGEVENTHANDLER_HPP

#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/CreateThreadDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractCreateThreadDebugEventHandler : public AbstractEventRecipient
{
private:
    std::list<std::pair<DWORD, ULONG_PTR>>& threads;
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

    virtual void HandleDebugEvent(const CreateThreadDebugEvent& event) = 0;

    virtual void LogCreateThread(DWORD tid, ULONG_PTR startAddress) const = 0;

    void AddThread(DWORD tid, ULONG_PTR startAddress)
    {
        threads.emplace_back(tid, startAddress);
    }

public:
    explicit AbstractCreateThreadDebugEventHandler(std::list<std::pair<DWORD, ULONG_PTR>>& threads,
                                                   std::optional<std::reference_wrapper<const Logger>> logger)
                                                   : threads(threads)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const CreateThreadDebugEvent&>(event));
    }

    ~AbstractCreateThreadDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTCREATETHREADDEBUGEVENTHANDLER_HPP
