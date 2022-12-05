//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTLOADDLLDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTLOADDLLDEBUGEVENTHANDLER_HPP

#include <list>
#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/LoadDllDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractLoadDllDebugEventHandler : public AbstractEventRecipient
{
private:
    std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries;
    std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries;
    std::optional<std::reference_wrapper<const Logger>> logger;

    void SetLogger(std::reference_wrapper<const Logger> loggerReference)
    {
        this->logger = loggerReference;
    }

protected:
    [[nodiscard]] virtual const std::optional<std::reference_wrapper<const Logger>>& GetLogger() const
    {
        return logger;
    }

    virtual void HandleDebugEvent(const LoadDllDebugEvent& event) = 0;

    virtual void LogLibraryLoad(const std::string& libraryName, ULONG_PTR baseAddress) const = 0;

    virtual void LogLibraryLoad(const std::wstring& libraryName, ULONG_PTR baseAddress) const = 0;

    void AddLibrary(const std::string& libraryName, ULONG_PTR baseAddress)
    {
        ansiLibraries.emplace_back(libraryName, baseAddress);
    }

    void AddLibrary(const std::wstring& libraryName, ULONG_PTR baseAddress)
    {
        unicodeLibraries.emplace_back(libraryName, baseAddress);
    }

public:
    explicit AbstractLoadDllDebugEventHandler(std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries,
                                              std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries,
                                              std::optional<std::reference_wrapper<const Logger>> logger)
            : ansiLibraries(ansiLibraries),
              unicodeLibraries(unicodeLibraries)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const LoadDllDebugEvent&>(event));
    }

    ~AbstractLoadDllDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTLOADDLLDEBUGEVENTHANDLER_HPP
