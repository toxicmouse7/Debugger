//
// Created by Aleksej on 02.12.2022.
//

#ifndef DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP
#define DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP

#include <Windows.h>
#include <sstream>
#include <list>

#include "Abstractions/Event Managing/AbstractEventRecipient.hpp"
#include "Debug Events/DebugEvent.hpp"
#include "Debug Events/DebugEventType.hpp"

class DefaultLoadDllDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries;
    std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries;

    void AddLibrary(const std::string& libraryName, ULONG_PTR baseAddress);

    void AddLibrary(const std::wstring& libraryName, ULONG_PTR baseAddress);

    static void LogLibraryLoad(const std::string& libraryName, ULONG_PTR baseAddress);

    static void LogLibraryLoad(const std::wstring& libraryName, ULONG_PTR baseAddress);

public:
    explicit DefaultLoadDllDebugEventHandler(std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries,
                                             std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries);

    void Handle(const AbstractEvent<DebugEventType>& event) override;
};


#endif //DEBUGGER_DEFAULTLOADDLLDEBUGEVENTHANDLER_HPP
