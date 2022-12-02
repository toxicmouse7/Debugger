//
// Created by Aleksej on 02.12.2022.
//

export module AbstractEventManager;

import std.core;
import AbstractEvent;
import AbstractEventRecipient;

export template<class AbstractEventType>
class AbstractEventManager
{
protected:
    std::list<AbstractEventRecipient<AbstractEventType>*> recipients;
public:
    virtual void NotifyAll(const AbstractEvent<AbstractEventType>& event) = 0;
    void AddRecipient(AbstractEventRecipient<AbstractEventType>* recipient)
    {
        recipients.push_back(recipient);
    };

    void RemoveRecipient(AbstractEventRecipient<AbstractEventType>* recipient)
    {
        recipients.remove(recipient);
    };
};
