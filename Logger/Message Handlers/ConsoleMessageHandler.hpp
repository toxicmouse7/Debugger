//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_CONSOLEMESSAGEHANDLER_HPP
#define DEBUGGER_CONSOLEMESSAGEHANDLER_HPP

#include <iostream>

#include "Abstractions/Message Handling/AbstractMessageHandler.hpp"

class ConsoleMessageHandler : public AbstractMessageHandler
{
public:
    void HandleMessage(const std::string& message) override;
    void HandleMessage(const std::wstring& message) override;
};

void ConsoleMessageHandler::HandleMessage(const std::string& message)
{
    std::cout << message << std::endl;
}

void ConsoleMessageHandler::HandleMessage(const std::wstring& message)
{
    std::wcout << message << std::endl;
}


#endif //DEBUGGER_CONSOLEMESSAGEHANDLER_HPP
