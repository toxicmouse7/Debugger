cmake_minimum_required(VERSION 3.25)
project(Debugger)

set(CMAKE_CXX_STANDARD 23)

set(CXX_MODULES_FLAGS /experimental:module /interface)

add_executable(Debugger main.cpp Debugger.ixx "Debug Events/DebugEventType.ixx" "Debug Events/DebugEvent.ixx" "Debug Event Hadlers/Default/DefaultCreateProcessDebugEventHandler.ixx" "Abstractions/Message Handling/AbstractMessageHandler.ixx" "Logger/Message Handlers/ConsoleMessageHandler.ixx" Logger/Logger.ixx "Debug Event Hadlers/Default/DefaultExceptionDebugEventHandler.ixx" "Debug Event Hadlers/Default/DefaultLoadDllDebugEventHandler.ixx" "Process Memory Manipultion/ProcessMemoryManipulation.ixx" "Abstractions/Event Managing/AbstractEvent.ixx" "Abstractions/Event Managing/AbstractEventManager.ixx" "Abstractions/Event Managing/AbstractEventRecipient.ixx" Utils/Utils.ixx "Debug Event Hadlers/Default/DefaultCreateThreadDebugEventHandler.ixx")

target_include_directories(Debugger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(Debugger PRIVATE ${CXX_MODULES_FLAGS})