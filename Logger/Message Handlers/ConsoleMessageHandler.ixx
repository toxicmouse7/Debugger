//
// Created by Aleksej on 02.12.2022.
//

export module ConsoleMessageHandler;

import std.core;
import AbstractMessageHandler;

export class ConsoleMessageHandler : public AbstractMessageHandler
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
