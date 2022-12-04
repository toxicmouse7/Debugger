//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_LOGGER_HPP
#define DEBUGGER_LOGGER_HPP

#include <string>
#include <map>
#include <list>
#include <memory>

#include "Abstractions/Message Handling/AbstractMessageHandler.hpp"

class Logger
{
private:
    inline static std::map<std::string, Logger*> loggers = {};

    Logger() = default;

    std::list<AbstractMessageHandler*> messageHandlers;

public:
    static Logger& GetInstance(const std::string& loggerName);

    static Logger& GetInstance(std::string&& loggerName);

    void Log(const std::string& message) const;

    void Log(const std::wstring& message) const;

    void AddMessageHandler(AbstractMessageHandler* messageHandler);

    ~Logger() = default;

    Logger(const Logger&) = delete;

    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;

    Logger& operator=(Logger&&) = delete;

};


#endif //DEBUGGER_LOGGER_HPP
