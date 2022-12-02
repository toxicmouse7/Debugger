cmake_minimum_required(VERSION 3.25)
project(Debugger)

set(CMAKE_CXX_STANDARD 23)

set(CXX_MODULES_FLAGS /experimental:module /interface)

add_executable(Debugger
        "Abstractions/Event Managing/AbstractEvent.ixx"
        "Abstractions/Event Managing/AbstractEventRecipient.ixx"
        "Abstractions/Event Managing/AbstractEventManager.ixx"
        "Debug Events/DebugEventType.ixx"
        "Debug Events/DebugEvent.ixx"
        "Process Memory Manipulation/ProcessMemoryManipulation.ixx"
        "Abstractions/Message Handling/AbstractMessageHandler.ixx"
        "Logger/Message Handlers/ConsoleMessageHandler.ixx"
        Logger/Logger.ixx
        Utils/Utils.ixx
        "Debug Event Handlers/Default/DefaultCreateProcessDebugEventHandler.ixx"
        "Debug Event Handlers/Default/DefaultExceptionDebugEventHandler.ixx"
        "Debug Event Handlers/Default/DefaultLoadDllDebugEventHandler.ixx"
        "Debug Event Handlers/Default/DefaultCreateThreadDebugEventHandler.ixx"
        Debugger.ixx
        main.cpp
        "Debug Event Handlers/Default/DefaultUnloadDllDebugEventHandler.ixx")

target_compile_options(Debugger PRIVATE ${CXX_MODULES_FLAGS})