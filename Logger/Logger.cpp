//
// Created by Aleksej on 02.12.2022.
//

#include "Logger.hpp"

Logger& Logger::GetInstance(const std::string& loggerName)
{
    if (!loggers.contains(loggerName))
    {
        loggers[loggerName] = new Logger();
    }

    return *loggers[loggerName];
}

void Logger::Log(const std::string& message) const
{
    for (auto& messageHandler : messageHandlers)
        messageHandler->HandleMessage(message);
}

void Logger::Log(const std::wstring& message) const
{
    for (auto& messageHandler : messageHandlers)
        messageHandler->HandleMessage(message);
}

void Logger::AddMessageHandler(AbstractMessageHandler* messageHandler)
{
    messageHandlers.push_back(messageHandler);
}

Logger& Logger::GetInstance(std::string&& loggerName)
{
    return GetInstance(loggerName);
}
