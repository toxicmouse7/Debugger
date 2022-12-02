//
// Created by Aleksej on 02.12.2022.
//

#include "Logger.hpp"

Logger* Logger::GetInstance()
{
    if (instance == nullptr)
        instance = new Logger();

    return instance;
}

void Logger::Log(const std::string& message)
{
    for (auto& messageHandler : messageHandlers)
        messageHandler->HandleMessage(message);
}

void Logger::Log(const std::wstring& message)
{
    for (auto& messageHandler : messageHandlers)
        messageHandler->HandleMessage(message);
}

void Logger::AddMessageHandler(AbstractMessageHandler* messageHandler)
{
    messageHandlers.push_back(messageHandler);
}
