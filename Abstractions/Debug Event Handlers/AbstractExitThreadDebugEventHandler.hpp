//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEXITTHREADDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTEXITTHREADDEBUGEVENTHANDLER_HPP


#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/ExitThreadDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractExitThreadDebugEventHandler : public AbstractEventRecipient
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

    virtual void HandleDebugEvent(const ExitThreadDebugEvent& event) = 0;

    virtual void LogExitThread(DWORD pid, DWORD tid, DWORD exitCode) const = 0;

    void RemoveThread(DWORD tid)
    {
        threads.remove_if([tid](const auto& thread) { return thread.first == tid; });
    }

public:
    explicit AbstractExitThreadDebugEventHandler(std::list<std::pair<DWORD, ULONG_PTR>>& threads,
                                                 std::optional<std::reference_wrapper<const Logger>> logger)
                                                 : threads(threads)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const ExitThreadDebugEvent&>(event));
    }

    ~AbstractExitThreadDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTEXITTHREADDEBUGEVENTHANDLER_HPP
