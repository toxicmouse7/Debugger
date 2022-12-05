//
// Created by Aleksej on 04.12.2022.
//

#include "DefaultBreakpointOnStartHandler.hpp"

void DefaultBreakpointOnStartHandler::Handle(const AbstractEvent& event)
{
    auto& createProcessDebugEvent = dynamic_cast<const CreateProcessDebugEvent&>(event);
    SetBreakpoint(reinterpret_cast<ULONG_PTR>(createProcessDebugEvent.payload.lpStartAddress));
}

DefaultBreakpointOnStartHandler::DefaultBreakpointOnStartHandler(
        const std::function<void(const ULONG_PTR&)>& setBreakpoint)
{
    this->SetBreakpoint = setBreakpoint;
}
