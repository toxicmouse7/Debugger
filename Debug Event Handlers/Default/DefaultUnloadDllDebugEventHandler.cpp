//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultUnloadDllDebugEventHandler.hpp"

void DefaultUnloadDllDebugEventHandler::HandleDebugEvent(const UnloadDllDebugEvent& event)
{
    auto library = FindLibrary(reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));

    if (library.has_value())
    {
        LogUnloadDll(library->get(), reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
        RemoveLibrary(library->get());
    }
    else
    {
        LogUnloadDll(L"(Unknown)", reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
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
