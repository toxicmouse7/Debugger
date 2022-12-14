logger_message_handler_path = '../Logger/Message Handlers'

logger_message_handler = """
#ifndef DEBUGGER_{inc_name}_HPP
#define DEBUGGER_{inc_name}_HPP

#include "Abstractions/Message Handling/AbstractMessageHandler.hpp"

class {name} : public AbstractMessageHandler
{{
public:
    void HandleMessage(const std::string& message) override;
    void HandleMessage(const std::wstring& message) override;
}};

void {name}::HandleMessage(const std::string& message)
{{
    
}}

void {name}::HandleMessage(const std::wstring& message)
{{
    
}}


#endif //DEBUGGER_{inc_name}_HPP

"""
