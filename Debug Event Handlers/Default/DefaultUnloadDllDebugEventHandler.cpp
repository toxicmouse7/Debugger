//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultUnloadDllDebugEventHandler.hpp"

void DefaultUnloadDllDebugEventHandler::HandleDebugEvent(const UnloadDllDebugEvent& event)
{
    auto foundAnsiLibrary = FindAnsiLibrary(reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
    auto foundUnicodeLibrary = FindUnicodeLibrary(reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));

    if (foundAnsiLibrary.has_value())
    {
        LogUnloadDll(foundAnsiLibrary->get(), reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
        RemoveLibrary(foundAnsiLibrary->get());
    }
    else if (foundUnicodeLibrary.has_value())
    {
        LogUnloadDll(foundUnicodeLibrary->get(), reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
        RemoveLibrary(foundUnicodeLibrary->get());
    }
    else
    {
        LogUnloadDll("(Unknown)", reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
    }
}

void DefaultUnloadDllDebugEventHandler::LogUnloadDll(const std::string& libraryName, ULONG_PTR baseAddress) const
{
    if (GetLogger().has_value())
    {
        std::stringstream stream;
        stream << "Library " << libraryName << " was unloaded from address " << Utils::FormatAddress(baseAddress, "'");
        GetLogger().value().get().Log(stream.str());
    }
}

void DefaultUnloadDllDebugEventHandler::LogUnloadDll(const std::wstring& libraryName, ULONG_PTR baseAddress) const
{
    if (GetLogger().has_value())
    {
        std::wstringstream stream;
        stream << L"Library " << libraryName << L" was unloaded from address "
               << Utils::FormatAddress(baseAddress, L"'");
        GetLogger().value().get().Log(stream.str());
    }
}
