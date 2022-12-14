//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTDEBUGEVENT_HPP
#define DEBUGGER_ABSTRACTDEBUGEVENT_HPP

#include "Abstractions/Event Managing/AbstractEvent.hpp"

template<class DebugEventPayload>
class AbstractDebugEvent : public AbstractEvent
{
public:
    unsigned int threadId;
    unsigned int processId;
    DebugEventPayload payload;

    AbstractDebugEvent(unsigned int threadId, unsigned int processId, DebugEventPayload payload)
            : threadId(threadId), processId(processId), payload(payload) {}

    ~AbstractDebugEvent() override = default;
};


#endif //DEBUGGER_ABSTRACTDEBUGEVENT_HPP
