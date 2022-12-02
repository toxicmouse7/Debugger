//
// Created by Aleksej on 02.12.2022.
//

module;

#include <Windows.h>

export module DefaultCreateThreadDebugEventHandler;

import std.core;

import AbstractEventRecipient;
import DebugEvent;
import Logger;

export class DefaultCreateThreadDebugEventHandler : public AbstractEventRecipient<DebugEventType>
{
private:
    std::list<std::pair<DWORD, ULONG_PTR>>& threads;

    static void LogThreadCreation(DWORD threadId, ULONG_PTR startAddress);

    void AddThread(DWORD threadId, ULONG_PTR startAddress);

public:
    explicit DefaultCreateThreadDebugEventHandler(std::list<std::pair<DWORD, ULONG_PTR>>& threads);

    void Handle(const AbstractEvent <DebugEventType>& event) override;
};

void DefaultCreateThreadDebugEventHandler::Handle(const AbstractEvent <DebugEventType>& event)
{
    if (event.eventType != DebugEventType::eCreateThread)
        return;

    auto createThreadEvent = dynamic_cast<const DebugEvent&>(event);

    auto createThreadData = static_cast<const CREATE_THREAD_DEBUG_INFO*>(event.GetData());

    LogThreadCreation(GetThreadId(createThreadData->hThread),
                      reinterpret_cast<ULONG_PTR>(createThreadData->lpStartAddress)
    );

    AddThread(GetThreadId(createThreadData->hThread), reinterpret_cast<ULONG_PTR>(createThreadData->lpStartAddress));
}

void DefaultCreateThreadDebugEventHandler::LogThreadCreation(DWORD threadId, ULONG_PTR startAddress)
{
    std::stringstream ss;
    ss << "Created thread with id: " << std::hex << threadId << " (Start address: " << std::hex << startAddress << ")";
    Logger::GetInstance()->Log(ss.str());
}

DefaultCreateThreadDebugEventHandler::DefaultCreateThreadDebugEventHandler(
        std::list<std::pair<DWORD, ULONG_PTR>>& threads) : threads(threads)
{

}

void DefaultCreateThreadDebugEventHandler::AddThread(DWORD threadId, ULONG_PTR startAddress)
{
    threads.emplace_back(threadId, startAddress);
}
