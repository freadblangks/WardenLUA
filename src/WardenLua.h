#ifndef MODULE_WARDENLUA_H
#define MODULE_WARDENLUA_H

#include "ScriptMgr.h"
#include "ChatCommand.h"
#include "Chat.h"
#include "Player.h"
#include "Config.h"

#include <Warden.h>
#include <WardenWin.h>

using namespace Acore::ChatCommands;

WorldPacket CreateAddonPacket(std::string const& msg, ChatMsg msgType, Player* player);
static void SendPayload(Player* player, std::string payload);

class WardenLuaCommands : public CommandScript
{
public:
    WardenLuaCommands() : CommandScript("WardenLuaCommands") { }

    ChatCommandTable GetCommands() const override;
    
    static bool HandleWLCPayload(ChatHandler* handler);
private:
    inline static uint32 payloadId= 800;
};

class WardenLuaPlayerScript : public PlayerScript
{
public:
    WardenLuaPlayerScript() : PlayerScript("WardenLuaPlayerScript") { }
    void OnLogin(Player* /*player*/) override;
};

#endif //MODULE_WARDENLUA_H
