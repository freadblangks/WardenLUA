#include "WardenLua.h"

ChatCommandTable WardenLuaCommands::GetCommands() const
{
    static ChatCommandTable sbCommandTable =
    {
        { "payload", HandleWLPayload, SEC_ADMINISTRATOR, Console::No },
        { "cpayload", HandleWLCPayload, SEC_ADMINISTRATOR, Console::No }
    };

    static ChatCommandTable commandTable =
    {
        { "wl", sbCommandTable }
    };

    return commandTable;
}

bool WardenLuaCommands::HandleWLCPayload(ChatHandler* handler)
{
    auto player = handler->GetPlayer();
    if (!player)
    {
        handler->SendSysMessage("You can only use this commands from ingame.");
        handler->SetSentErrorMessage(true);
        return false;
    }

    std::string payload = "local wocTest = 'Hello World 2'; print(GetTime()..wocTest); return false;";
    LOG_INFO("Trying to send payload: '{}'", payload);

    handler->SendSysMessage(Acore::StringFormatFmt("Hello World!, Payload: {}", payload));

    auto warden = player->GetSession()->GetWarden()->get();
    auto wardenWin = (WardenWin*)warden;

    if (!wardenWin)
    {
        handler->SendSysMessage("Failed to find Warden from session.");
        handler->SetSentErrorMessage(true);
        return false;
    }

    wardenWin->SendLuaPayload(payload);
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

    LOG_INFO("Trying to send payload: '{}'", payload);

    handler->SendSysMessage(Acore::StringFormatFmt("Hello World!, Payload: {}", payload));

    auto warden = player->GetSession()->GetWarden()->get();
    auto wardenWin = (WardenWin*)warden;

    if (!wardenWin)
    {
        handler->SendSysMessage("Failed to find Warden from session.");
        handler->SetSentErrorMessage(true);
        return false;
    }

    wardenWin->SendLuaPayload(payload);

    return true;
}

void AddSCWardenLUAScripts()
{
    new WardenLuaCommands();
}
