#include "../Util/pch.h"
#include "RoomBase.h"

{%- for pkt in parser.recv_pkt %}
void RoomBase::Handle_{{pkt.name}}(shared_ptr<GameSession>& session, Protocol::{{pkt.name}}& pkt)
{
    for (auto it = {{pkt.name}}_Handlers.begin(); it != {{pkt.name}}_Handlers.end(); it++)
    {
        (*it)(session, pkt);
    }
}
{%- endfor %}