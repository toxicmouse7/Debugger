
#ifndef DEBUGGER_FILEMESSAGEHANDLER_HPP
#define DEBUGGER_FILEMESSAGEHANDLER_HPP

#include <fstream>
#include "Abstractions/Message Handling/AbstractMessageHandler.hpp"

class FileMessageHandler : public AbstractMessageHandler
{
private :
    std::wofstream fileStream;
public:
    void HandleMessage(const std::string& message) override;

    void HandleMessage(const std::wstring& message) override;

    explicit FileMessageHandler(const std::string& filePath);
};

void FileMessageHandler::HandleMessage(const std::string& message)
{
    fileStream << message.c_str() << std::endl;
}

void FileMessageHandler::HandleMessage(const std::wstring& message)
{
    fileStream << message << std::endl;
}

FileMessageHandler::FileMessageHandler(const std::string& filePath)
{
    fileStream.open(filePath);

    if (!fileStream.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }
}


#endif //DEBUGGER_FILEMESSAGEHANDLER_HPP

