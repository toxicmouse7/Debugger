//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP
#define DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP


#include "AbstractEvent.hpp"

class AbstractEventRecipient
{
public:
    virtual void Handle(const AbstractEvent& event) = 0;
    virtual ~AbstractEventRecipient() = default;
};


#endif //DEBUGGER_ABSTRACTEVENTRECIPIENT_HPP
