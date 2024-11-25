/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OUTDOOR_PVP_MGR_H_
#define OUTDOOR_PVP_MGR_H_

#define OUTDOORPVP_OBJECTIVE_UPDATE_INTERVAL 1000

#include "OutdoorPvP.h"

class Player;
class GameObject;
class Creature;
class ZoneScript;
struct GossipMenuItems;

struct OutdoorPvPData
{
    OutdoorPvPTypes TypeId;
    uint32 ScriptId;
};

// class to handle player enter / leave / areatrigger / GO use events
class OutdoorPvPMgr
{
private:
    OutdoorPvPMgr();
    ~OutdoorPvPMgr() = default;

public:
    static OutdoorPvPMgr* instance();

    // create outdoor pvp events
    void InitOutdoorPvP();

    // cleanup
    void Die();

    // called when a player enters an outdoor pvp area
    void HandlePlayerEnterZone(Player* player, uint32 areaflag);

    // called when player leaves an outdoor pvp area
    void HandlePlayerLeaveZone(Player* player, uint32 areaflag);

    // called when player resurrects
    void HandlePlayerResurrects(Player* player, uint32 areaflag);

    // return assigned outdoor pvp
    OutdoorPvP* GetOutdoorPvPToZoneId(uint32 zoneid);

    // handle custom (non-exist in dbc) spell if registered
    bool HandleCustomSpell(Player* player, uint32 spellId, GameObject* go);

    // handle custom go if registered
    bool HandleOpenGo(Player* player, GameObject* go);

    ZoneScript* GetZoneScript(uint32 zoneId);

    void AddZone(uint32 zoneid, OutdoorPvP* handle);

    void Update(uint32 diff);

    void HandleGossipOption(Player* player, Creature* creatured, uint32 gossipid);

    bool CanTalkTo(Player* player, Creature* creature, GossipMenuItems const& gso);

    void HandleDropFlag(Player* player, uint32 spellId);

    // pussywizard: lock required because different functions affect _players
    std::mutex _lock;

private:
    // contains all initiated outdoor pvp events
    // used when initing / cleaning up
    std::vector<std::unique_ptr<OutdoorPvP>> m_OutdoorPvPSet;

    // maps the zone ids to an outdoor pvp event
    // used in player event handling
    std::map<uint32/*zoneid*/, OutdoorPvP*> m_OutdoorPvPMap;

    // Holds the outdoor PvP templates
    std::map<OutdoorPvPTypes, std::unique_ptr<OutdoorPvPData>> m_OutdoorPvPDatas;

    // update interval
    uint32 m_UpdateTimer;
};

#define sOutdoorPvPMgr OutdoorPvPMgr::instance()

#endif /*OUTDOOR_PVP_MGR_H_*/
