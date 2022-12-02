//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_ABSTRACTMESSAGEHANDLER_HPP
#define DEBUGGER_ABSTRACTMESSAGEHANDLER_HPP

#include <string>

class AbstractMessageHandler
{
public:
    virtual void HandleMessage(const std::string& message) = 0;
    virtual void HandleMessage(const std::wstring& message) = 0;
};


#endif //DEBUGGER_ABSTRACTMESSAGEHANDLER_HPP
