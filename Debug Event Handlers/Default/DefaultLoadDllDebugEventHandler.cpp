//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultLoadDllDebugEventHandler.hpp"
#include "Utils/Utils.hpp"

void DefaultLoadDllDebugEventHandler::HandleDebugEvent(const LoadDllDebugEvent& event)
{
    if (event.payload.lpImageName != nullptr)
    {
        ULONG_PTR pImageName = ProcessMemoryManipulation::ReadPointer(
                event.processId, reinterpret_cast<ULONG_PTR>(event.payload.lpImageName));

        auto dllName = Utils::Split(ProcessMemoryManipulation::ReadWString(
                event.processId, pImageName
        ), L"\\").back();
        std::ranges::transform(dllName, dllName.begin(), [](auto c) { return std::tolower(c); });
        LogLibraryLoad(dllName, reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
        AddLibrary(dllName, reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
    }
    else
    {
        LogLibraryLoad(L"(Unknown)", reinterpret_cast<ULONG_PTR>(event.payload.lpBaseOfDll));
    }
}

DefaultLoadDllDebugEventHandler::DefaultLoadDllDebugEventHandler(
        std::map<std::wstring, ULONG_PTR>& libraries,
        std::optional<std::reference_wrapper<const Logger>> logger) :
        AbstractLoadDllDebugEventHandler(libraries, logger)
{

}

void DefaultLoadDllDebugEventHandler::LogLibraryLoad(const std::wstring& libraryName, ULONG_PTR baseAddress) const
{
    if (GetLogger().has_value())
    {
        std::wstringstream ss;
        ss << L"Loaded library: " << libraryName << L" at " << Utils::FormatAddress(baseAddress, L"'");
        GetLogger().value().get().Log(ss.str());
    }
}
