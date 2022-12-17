#ifndef MODULE_WARDENLUA_H
#define MODULE_WARDENLUA_H

#include "ScriptMgr.h"
#include "ChatCommand.h"
#include "Chat.h"

using namespace Acore::ChatCommands;

class WardenLuaCommands : public CommandScript
{
public:
    WardenLuaCommands() : CommandScript("WardenLuaCommands") { }

    ChatCommandTable GetCommands() const override;
    static bool HandleWLPayload(ChatHandler* handler, std::string payload);
};

#endif //MODULE_WARDENLUA_H
