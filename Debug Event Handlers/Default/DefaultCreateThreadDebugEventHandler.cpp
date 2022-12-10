//
// Created by Aleksej on 03.12.2022.
//

#include "DefaultCreateThreadDebugEventHandler.hpp"

void DefaultCreateThreadDebugEventHandler::HandleDebugEvent(const CreateThreadDebugEvent& event)
{
    AddThread(event.threadId, reinterpret_cast<ULONG_PTR>(event.payload.lpStartAddress));

    LogCreateThread(event.threadId, reinterpret_cast<ULONG_PTR>(event.payload.lpStartAddress));
}

void DefaultCreateThreadDebugEventHandler::LogCreateThread(DWORD tid, ULONG_PTR startAddress) const
{
    if (GetLogger().has_value())
    {
        std::stringstream ss;
        ss << "Thread " << tid << " created with start address " << Utils::FormatAddress(startAddress, "'");
        GetLogger().value().get().Log(ss.str());
    }
}

DefaultCreateThreadDebugEventHandler::DefaultCreateThreadDebugEventHandler(
        std::map<DWORD, ULONG_PTR>& threads, std::optional<std::reference_wrapper<const Logger>> logger)
        : AbstractCreateThreadDebugEventHandler(threads, logger)
{

}
