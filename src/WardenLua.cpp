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
    auto player = handler->GetPlayer();
    if (!player)
    {
        handler->SendSysMessage("You can only use this commands from ingame.");
        handler->SetSentErrorMessage(true);
        return false;
    }

    handler->SendSysMessage(Acore::StringFormatFmt("Hello World!, Payload: {}", payload));

    auto warden = WardenWin::GetInstance();
    warden->SendLuaPayload(player->GetSession(), payload);

    return true;
}

void AddSCWardenLUAScripts()
{
    new WardenLuaCommands();
}
