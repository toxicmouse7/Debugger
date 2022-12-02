//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEVENTMANAGER_HPP
#define DEBUGGER_ABSTRACTEVENTMANAGER_HPP

#include <list>

#include "AbstractEventRecipient.hpp"
#include "AbstractEvent.hpp"

template<class AbstractEventType>
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


#endif //DEBUGGER_ABSTRACTEVENTMANAGER_HPP
