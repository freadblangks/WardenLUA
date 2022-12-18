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

void SendPayload(Player* player, uint32 payloadId, std::string payload)
{
    SendPayload(player->GetSession(), payloadId, payload);
}

void SendPayload(WorldSession* session, uint32 payloadId, std::string payload)
{
    auto warden = session->GetWarden()->get();
    auto wardenWin = (WardenWin*)warden;

    if (!wardenWin)
    {
        return;
    }

    wardenWin->RequestChecks();
    wardenWin->QueueLuaPayload(payloadId, payload);
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
    
    std::string payload = "local wocTest = 'Hello World 2'; message(GetTime()..wocTest); return false;";
    LOG_INFO("module", "Trying to send payload: '{}'", payload);

    handler->SendSysMessage(Acore::StringFormatFmt("Hello World!, Payload: {}", payload));

    SendPayload(player, 800, payload);

    return true;
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

    LOG_INFO("module", "Trying to send payload: '{}'", payload);

    handler->SendSysMessage(Acore::StringFormatFmt("Hello World!, Payload: {}", payload));

    SendPayload(player, 800, payload);

    return true;
}

void WardenLuaPlayerScript::OnLogin(Player* player)
{
    if (!player)
    {
        return;
    }

    std::string payload = "message('Welcome to the server!|n|n|cffffffffEnjoy your stay.'); return false;";

    SendPayload(player, 800, payload);
}

bool passed = false;

bool WardenLuaServerScript::CanPacketReceive(WorldSession* session, WorldPacket& packet)
{
    switch (packet.GetOpcode())
    {
    case CMSG_UPDATE_ACCOUNT_DATA:
        passed = true;
        break;

    case CMSG_WARDEN_DATA:
        if (passed)
        {
            std::string payload = "AddonList:Show();return false;";
            SendPayload(session, 800, payload);
            passed = false;
        }
        break;
    }
    LOG_INFO("module", "Received packet type: {}", packet.GetOpcode());

    return true;
}

void AddSCWardenLUAScripts()
{
    new WardenLuaServerScript();
    new WardenLuaCommands();
    new WardenLuaPlayerScript();
}
