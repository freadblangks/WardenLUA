#include "WardenLua.h"

ChatCommandTable WardenLuaCommands::GetCommands() const
{
    static ChatCommandTable sbCommandTable =
    {
        { "payload", HandleWLCPayload, SEC_ADMINISTRATOR, Console::No }
    };

    static ChatCommandTable commandTable =
    {
        { "wl", sbCommandTable }
    };

    return commandTable;
}

void SendPayload(Player* player, std::string payload)
{
    WorldPacket pPayload = CreateAddonPacket(payload, CHAT_MSG_WHISPER);
    player->SendDirectMessage(&pPayload);
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

    SendPayload(player, payload);

    return true;
}

WorldPacket CreateAddonPacket(std::string const& msg, ChatMsg msgType, Player* player)
{
    std::string prefix("ws");
    WorldPacket data;
    size_t len = msg.length();
    size_t preLen = prefix.length();

    data.Initialize(SMSG_MESSAGECHAT, 1 + 4 + 8 + 4 + 8 + 4 + 1 + len + 1);
    data << uint8(msgType); //Type
    data << uint32(LANG_ADDON); //Lang
    data << uint64(); //SenderGUID
    data << uint32(0); //Flags
    data << uint64(0); 
    data << uint32(len + 1); //MsgLen
    data << msg; //Msg
    data << uint32(preLen + 1);
    data << prefix;
    data << uint8(0);

    return data;
}

void WardenLuaPlayerScript::OnLogin(Player* player)
{
    if (!player)
    {
        return;
    }

    std::string payload = "message('Welcome to the server!')";

    SendPayload(player, payload);
}

void AddSCWardenLUAScripts()
{
    if (sConfigMgr->GetOption<bool>("WardenLua.Enable", false))
    {
        new WardenLuaCommands();
        new WardenLuaPlayerScript();
    }
}
