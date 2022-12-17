#include "WardenLua.h"

ChatCommandTable WardenLuaCommands::GetCommands() const
{
    static ChatCommandTable sbCommandTable =
    {
        { "payload", HandleWLPayload, SEC_ADMINISTRATOR, Console::No }
    };

    static ChatCommandTable commandTable =
    {
        { "wl", sbCommandTable }
    };

    return commandTable;
}


bool WardenLuaCommands::HandleWLPayload(ChatHandler* handler, std::string payload)
{
    handler->SendSysMessage("Hello World!");
}

void SCWardenLuaScripts()
{
    new WardenLuaCommands();
}
