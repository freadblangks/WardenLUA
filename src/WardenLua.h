#ifndef MODULE_WARDENLUA_H
#define MODULE_WARDENLUA_H

#include "ScriptMgr.h"
#include "ChatCommand.h"
#include "Chat.h"
#include "Player.h"

#include <Warden.h>
#include <WardenWin.h>

using namespace Acore::ChatCommands;

static constexpr char _luaEvalPrefix[] = "local S,T,R=SendAddonMessage,function()";
static constexpr char _luaEvalMidfix[] = " end R=S and T()if R then S('_TW',";
static constexpr char _luaEvalPostfix[] = ",'GUILD')end";

class WardenLuaCommands : public CommandScript
{
public:
    WardenLuaCommands() : CommandScript("WardenLuaCommands") { }

    ChatCommandTable GetCommands() const override;
    static bool HandleWLPayload(ChatHandler* handler, std::string payload);
};

#endif //MODULE_WARDENLUA_H
