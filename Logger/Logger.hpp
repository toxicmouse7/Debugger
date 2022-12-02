//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_LOGGER_HPP
#define DEBUGGER_LOGGER_HPP

#include <string>
#include <list>

#include "Abstractions/Message Handling/AbstractMessageHandler.hpp"

class Logger
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


#endif //DEBUGGER_LOGGER_HPP
