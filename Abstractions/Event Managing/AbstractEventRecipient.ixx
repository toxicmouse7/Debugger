//
// Created by Aleksej on 02.12.2022.
//

export module AbstractEventRecipient;

import AbstractEvent;

export template<class AbstractEventType>
class AbstractEventRecipient
{
public:
    virtual void Handle(const AbstractEvent<AbstractEventType>& event) = 0;
};
