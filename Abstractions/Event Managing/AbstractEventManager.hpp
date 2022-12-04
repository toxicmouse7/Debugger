//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEVENTMANAGER_HPP
#define DEBUGGER_ABSTRACTEVENTMANAGER_HPP

#include <map>
#include <list>

#include "AbstractEventRecipient.hpp"
#include "AbstractEvent.hpp"

template<class RecipientType>
class AbstractEventManager
{
protected:
    std::map<RecipientType, std::list<AbstractEventRecipient*>> recipients;

    void AddRecipient(RecipientType recipientType, AbstractEventRecipient* recipient)
    {
        recipients[recipientType].push_back(recipient);
    };

    void RemoveRecipient(RecipientType recipientType, AbstractEventRecipient* recipient)
    {
        recipients[recipientType].remove(recipient);
    };

public:
    virtual void Notify(RecipientType recipientType, const AbstractEvent& event) = 0;

    virtual ~AbstractEventManager() = default;
};


#endif //DEBUGGER_ABSTRACTEVENTMANAGER_HPP
