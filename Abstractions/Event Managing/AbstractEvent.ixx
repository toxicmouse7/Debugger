//
// Created by Aleksej on 02.12.2022.
//

export module AbstractEvent;

export template <class AbstractEventType>
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

