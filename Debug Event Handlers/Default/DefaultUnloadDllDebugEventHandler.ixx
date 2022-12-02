//
// Created by Aleksej on 03.12.2022.
//

module;

#include <Windows.h>

export module DefaultUnloadDllDebugEventHandler;

import std.core;

import AbstractEventRecipient;
import DebugEventType;
import AbstractEvent;
import DebugEvent;

export class DefaultUnloadDllDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries;
    std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries;
public:
    DefaultUnloadDllDebugEventHandler(std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries,
                                      std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries);

    void Handle(const AbstractEvent <DebugEventType>& event) override;
};

void DefaultUnloadDllDebugEventHandler::Handle(const AbstractEvent <DebugEventType>& event)
{
    if (event.eventType != DebugEventType::eUnloadDll)
        return;

    auto unloadDllEvent = dynamic_cast<const DebugEvent&>(event);

    auto unloadDllData = static_cast<const UNLOAD_DLL_DEBUG_INFO*>(event.GetData());

    auto nameAndAddress = std::find_if(ansiLibraries.begin(), ansiLibraries.end(),
                                               [unloadDllData](const auto& pair)
                                               {
                                                   return pair.second ==
                                                          reinterpret_cast<ULONG_PTR>(unloadDllData->lpBaseOfDll);
                                               });
}

DefaultUnloadDllDebugEventHandler::DefaultUnloadDllDebugEventHandler(
        std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries,
        std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries) :
        ansiLibraries(ansiLibraries),
        unicodeLibraries(unicodeLibraries)
{

}
