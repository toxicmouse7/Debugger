//
// Created by Aleksej on 02.12.2022.
//

module;

#include <Windows.h>

export module DefaultLoadDllDebugEventHandler;

import std.core;

import AbstractEventRecipient;
import DebugEvent;
import ProcessMemoryManipulation;
import Utils;
import Logger;

export class DefaultLoadDllDebugEventHandler : public AbstractEventRecipient<DebugEventType>
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

void DefaultLoadDllDebugEventHandler::Handle(const AbstractEvent<DebugEventType>& event)
{
    if (event.eventType != DebugEventType::eLoadDll)
        return;

    auto loadDllEvent = dynamic_cast<const DebugEvent&>(event);

    auto loadDllData = static_cast<const LOAD_DLL_DEBUG_INFO*>(event.GetData());

    if (loadDllData->lpImageName != nullptr)
    {
        void* pImageName = ProcessMemoryManipulation::ReadPointer(
                loadDllEvent.processHandle, loadDllData->lpImageName
        );

        if (loadDllData->fUnicode == 0)
        {
            auto dllName = ProcessMemoryManipulation::ReadString(
                    loadDllEvent.processHandle, pImageName
            );
            LogLibraryLoad(dllName, reinterpret_cast<ULONG_PTR>(loadDllData->lpBaseOfDll));
        }
        else
        {
            auto dllName = ProcessMemoryManipulation::ReadWString(
                    loadDllEvent.processHandle, pImageName
            );
            LogLibraryLoad(dllName, reinterpret_cast<ULONG_PTR>(loadDllData->lpBaseOfDll));
        }
    }
    else
    {
        LogLibraryLoad("(Unknown)", reinterpret_cast<ULONG_PTR>(loadDllData->lpBaseOfDll));
    }

}

DefaultLoadDllDebugEventHandler::DefaultLoadDllDebugEventHandler(
        std::list<std::pair<std::string, ULONG_PTR>>& ansiLibraries,
        std::list<std::pair<std::wstring, ULONG_PTR>>& unicodeLibraries) : ansiLibraries(ansiLibraries),
                                                                           unicodeLibraries(unicodeLibraries)
{

}

void DefaultLoadDllDebugEventHandler::AddLibrary(const std::string& libraryName, ULONG_PTR baseAddress)
{
    ansiLibraries.emplace_back(Utils::Split(libraryName, "\\").back(), baseAddress);
}

void DefaultLoadDllDebugEventHandler::AddLibrary(const std::wstring& libraryName, ULONG_PTR baseAddress)
{
    unicodeLibraries.emplace_back(Utils::Split(libraryName, L"\\").back(), baseAddress);
}

void DefaultLoadDllDebugEventHandler::LogLibraryLoad(const std::string& libraryName, ULONG_PTR baseAddress)
{
    std::stringstream ss;
    ss << "Loaded DLL: " << Utils::Split(libraryName, "\\").back() << ' ';
    ss << " (Base address: " << std::hex << baseAddress << ")";
    Logger::GetInstance()->Log(ss.str());
}

void DefaultLoadDllDebugEventHandler::LogLibraryLoad(const std::wstring& libraryName, ULONG_PTR baseAddress)
{
    std::wstringstream ss;
    ss << L"Loaded DLL: " << Utils::Split(libraryName, L"\\").back() << ' ';
    ss << L" (Base address: " << std::hex << baseAddress << ")";
    Logger::GetInstance()->Log(ss.str());
}
