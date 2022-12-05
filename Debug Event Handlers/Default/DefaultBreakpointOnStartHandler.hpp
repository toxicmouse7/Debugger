//
// Created by Aleksej on 04.12.2022.
//

#ifndef DEBUGGER_DEFAULTBREAKPOINTONSTARTHANDLER_HPP
#define DEBUGGER_DEFAULTBREAKPOINTONSTARTHANDLER_HPP

#include <functional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/CreateProcessDebugEvent.hpp"
#include "Debugger/Breakpoint.hpp"

class DefaultBreakpointOnStartHandler : public AbstractEventRecipient
{
private:
    std::function<void(const ULONG_PTR&)> SetBreakpoint;
    void Handle(const AbstractEvent& event) override;
public:

    explicit DefaultBreakpointOnStartHandler(const std::function<void(const ULONG_PTR&)>& setBreakpoint);

    ~DefaultBreakpointOnStartHandler() override = default;
};


#endif //DEBUGGER_DEFAULTBREAKPOINTONSTARTHANDLER_HPP
