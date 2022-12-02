//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP
#define DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP


#include "AbstractEvent.hpp"

template<class AbstractEventType>
class AbstractEventRecipient
{
public:
    virtual void Handle(const AbstractEvent<AbstractEventType>& event) = 0;
};


#endif //DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP
