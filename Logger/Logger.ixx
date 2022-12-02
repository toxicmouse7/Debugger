//
// Created by Aleksej on 02.12.2022.
//

export module Logger;

import std.core;
import AbstractMessageHandler;

export class Logger
{
private:
    inline static Logger* instance = nullptr;

    Logger() = default;

    std::list<AbstractMessageHandler*> messageHandlers;

public:
    static Logger* GetInstance();

    void Log(const std::string& message);

    void Log(const std::wstring& message);

    void AddMessageHandler(AbstractMessageHandler* messageHandler);

    ~Logger() = default;

    Logger(const Logger&) = delete;

    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;

    Logger& operator=(Logger&&) = delete;

};

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
