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

    wardenWin->QueueLuaPayload(payloadId, Acore::StringFormatFmt("{};return false;", payload));
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
    
    std::string payload = "message(GetTime()..' Hello World!')";

    SendPayload(player, 800, payload);

    return true;
}

WorldPacket CreateAddonPacket(std::string const& msg, ChatMsg msgType)
{
    WorldPacket data;
    size_t len = msg.length();
    data.Initialize(SMSG_MESSAGECHAT, 1 + 4 + 8 + 4 + 8 + 4 + 1 + len + 1);
    data << uint8(msgType);
    data << uint32(LANG_ADDON);
    data << uint64(0);
    data << uint32(0);
    data << uint64(0);
    data << uint32(len + 1);
    data << msg;
    data << uint8(0);

    return data;
}

bool WardenLuaCommands::HandleWLPayload(ChatHandler* handler, uint32 repeat)
{
    auto player = handler->GetPlayer();
    if (!player)
    {
        handler->SendSysMessage("You can only use this commands from ingame.");
        handler->SetSentErrorMessage(true);
        return false;
    }

    WorldPacket addonPacket = CreateAddonPacket("message('hello world!')", ChatMsg::CHAT_MSG_WHISPER);
    player->SendDirectMessage(&addonPacket);

    return true;
}

void WardenLuaPlayerScript::OnLogin(Player* player)
{
    if (!player)
    {
        return;
    }

    std::string payload = "WorldFrame:RegisterEvent(\"CHAT_MSG_ADDON\")WorldFrame:SetScript(\"OnEvent\",function(self,b,...)if b==\"CHAT_MSG_ADDON\"then a=...loadstring(a)()end end)";

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
            std::string payload = "message('test')";
            //SendPayload(session, 800, payload);
            passed = false;
        }
        break;
    }
    //LOG_INFO("module", "Received packet type: {}", packet.GetOpcode());

    return true;
}

void AddSCWardenLUAScripts()
{
    if (sConfigMgr->GetOption<bool>("WardenLua.Enable", false))
    {
        new WardenLuaServerScript();
        new WardenLuaCommands();
        new WardenLuaPlayerScript();
    }
}
