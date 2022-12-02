//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEVENT_HPP
#define DEBUGGER_ABSTRACTEVENT_HPP

template <class AbstractEventType>
class AbstractEvent
{
protected:
    const void* data = nullptr;
public:
    const AbstractEventType eventType;

    AbstractEvent(const AbstractEventType& eventType, const void* data) : eventType(eventType), data(data) {}

    [[nodiscard]] const void* GetData() const
    {
        return data;
    }

    virtual ~AbstractEvent() = default;
};


#endif //DEBUGGER_ABSTRACTEVENT_HPP
