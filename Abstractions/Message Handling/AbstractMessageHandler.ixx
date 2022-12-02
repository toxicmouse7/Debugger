//
// Created by Aleksej on 02.12.2022.
//
export module AbstractMessageHandler;

import std.core;

export class AbstractMessageHandler
{
public:
    virtual void HandleMessage(const std::string& message) = 0;
    virtual void HandleMessage(const std::wstring& message) = 0;
};
