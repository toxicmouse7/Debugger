//
// Created by Aleksej on 03.12.2022.
//

#ifndef DEBUGGER_ABSTRACTUNLOADDLLDEBUGEVENTHANDLER_HPP
#define DEBUGGER_ABSTRACTUNLOADDLLDEBUGEVENTHANDLER_HPP

#include <list>
#include <sstream>
#include <optional>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/UnloadDllDebugEvent.hpp"
#include "Logger/Logger.hpp"
#include "Utils/Utils.hpp"

class AbstractUnloadDllDebugEventHandler : public AbstractEventRecipient
{
private:
    std::map<std::wstring, ULONG_PTR>& libraries;
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

    virtual void HandleDebugEvent(const UnloadDllDebugEvent& event) = 0;

    virtual void LogUnloadDll(const std::wstring& libraryName, ULONG_PTR baseAddress) const = 0;

    [[nodiscard]] std::optional<std::reference_wrapper<const std::wstring>> FindLibrary(ULONG_PTR address) const
    {
        for (const auto& library: libraries)
        {
            if (library.second == address)
                return library.first;
        }

        return std::nullopt;
    }

    void RemoveLibrary(const std::wstring& libraryName)
    {
        libraries.erase(libraryName);
    }

public:
    explicit AbstractUnloadDllDebugEventHandler(std::map<std::wstring, ULONG_PTR>& libraries,
                                                std::optional<std::reference_wrapper<const Logger>> logger)
            : libraries(libraries)
    {
        if (logger.has_value())
            SetLogger(logger.value());
    }

    void Handle(const AbstractEvent& event) final
    {
        this->HandleDebugEvent(dynamic_cast<const UnloadDllDebugEvent&>(event));
    }

    ~AbstractUnloadDllDebugEventHandler() override = default;
};


#endif //DEBUGGER_ABSTRACTUNLOADDLLDEBUGEVENTHANDLER_HPP
