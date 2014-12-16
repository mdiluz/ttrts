#include "unit.h"

#include <map> // for std::map

namespace
{	
	// Helper function for generating unique unit ids during static init
	unit_id_t get_unique_unit_id()
	{
		static unit_id_t p = 0;
		return p++;
	}

	// Map of visual representation of unit V
	typedef std::map< dir_t, unitVis_c > dir_to_vis_map;
	
	// Helper function to get the vis map during static init
	const dir_to_vis_map& get_vis_map_V()
	{
		static const dir_to_vis_map sk_visMap =
		{
			{dir_t::N,'^'},
			{dir_t::E,'>'},
			{dir_t::S,'v'},
			{dir_t::W,'<'},
		};

		return sk_visMap;
	}
}

// Get a unit from a visual
CUnit GetUnitFromVis( unitVis_c vis )
{
    CUnit unit;
    unit.setFromVisual(vis);
    return unit;
}

// Get a string descriptor of a unit
// "U:[unit_id] T:[team_id] P:[player_id] V:[unit_vis] D:[dir] POS:[pos.x],[pos.y]"
std::string GetStringFromUnit(const CUnit& unit )
{
    std::string ret;
    ret += "U:";
    ret += std::to_string(unit.getID());
    ret += " T:";
    ret += std::to_string((int)unit.getTeam());
    ret += " P:";
    ret += std::to_string(unit.getPlayer());
    ret += " V:";
    ret += (char)unit.getVisual();
    ret += " D:";
    ret += (char)unit.getDir();
    ret += " POS:";
    ret += std::to_string(unit.getPos().x);
    ret += ",";
    ret += std::to_string(unit.getPos().y);

    return ret;
}

// Get a unit from a string descriptor
// "U:[unit_id] T:[team_id] P:[player_id] V:[unit_vis] D:[dir] POS:[pos.x],[pos.y]"
CUnit GetUnitFromString(const std::string& _unit )
{
    std::string unit = _unit;
    CUnit ret;
    size_t pos;

    // UNIT ID
    if( unit.substr(0, 2) != "U:" )
        return CUnit();
    unit.erase(0, 2); // Erase the U: prefix

    pos = unit.find(' ');
    if( pos == std::string::npos )
        return CUnit();

    const unit_id_t id = (unit_id_t)atoi( unit.substr(0,pos).c_str() ); // Grab the ID
    ret.setIDForced( id ); // Force set the ID
    unit.erase(0, pos+1); // Erase the ID and a space

    // TEAM
    if( unit.substr(0, 2) != "T:" )
        return CUnit();
    unit.erase(0, 2); // Erase the T: prefix

    pos = unit.find(' ');
    if( pos == std::string::npos )
        return CUnit();

    const Team team = (Team)atoi( unit.substr(0,pos).c_str() ); // Grab the ID
    ret.setTeam( team ); // Force set the ID
    unit.erase(0, pos+1); // Erase the team and a space

    // PLAYER
    if( unit.substr(0, 2) != "P:" )
        return CUnit();
    unit.erase(0, 2); // Erase the P: prefix

    pos = unit.find(' ');
    if( pos == std::string::npos )
        return CUnit();

    const player_id_t player = (player_id_t)atoi( unit.substr(0,pos).c_str() ); // Grab the ID
    ret.setPlayer( player ); // Force set the ID
    unit.erase(0, pos+1); // Erase the player and a space

    // VISUAL
    if( unit.substr(0, 2) != "V:" )
        return CUnit();
    unit.erase(0, 2); // Erase the W: prefix

    unitVis_c vis = unit[0];
    ret.setVisual(vis);
    unit.erase(0, 2); // Erase the vis and a space

    // DIRECTION
    if( unit.substr(0, 2) != "D:" )
        return CUnit();
    unit.erase(0, 2); // Erase the D: prefix

    dir_t dir = (dir_t)unit[0];
    ret.setDir(dir);
    unit.erase(0, 2); // Erase the ID and a space

    // POSITION
    if( unit.substr(0, 4) != "POS:" )
        return CUnit();
    unit.erase(0, 4); // Erase the Pos: prefix

    pos = unit.find(',');
    if( pos == std::string::npos )
        return CUnit();

    uvector2 upos;
    upos.x = (ucoord_t)atoi( unit.substr(0,pos).c_str() ); // Grab the x
    unit.erase(0, pos+1); // Erase the x and the comma

    upos.y = (ucoord_t)atoi( unit.c_str() ); // Grab the y from what's left
    ret.setPos( upos ); //Set the position

    return ret;
}

// Plain constructor
CUnit::CUnit()
: unit_id 	( get_unique_unit_id() )
, team_id 	( Team::NUM_INVALID )
, unit_vis 	( unitVis_invalid )
, player_id ( player_id_invalid )
, dir 		( dir_t::S )
, pos 		( { ucoord_invalid, ucoord_invalid } )
{
	updateMyVisual();
}

// Move constructor
CUnit::CUnit(CUnit&& unit)
: unit_id 	( std::move(unit.unit_id) )
, team_id 	( std::move(unit.team_id) )
, player_id ( std::move(unit.player_id) )
, unit_vis 	( std::move(unit.unit_vis) )
, dir 		( std::move(unit.dir) )
, pos 		( std::move(unit.pos) )
{
	updateMyVisual();
}


// Move asignment operator
CUnit& CUnit::operator=(CUnit&& unit)
{
    unit_id 	= std::move(unit.unit_id) ;
    team_id 	= std::move(unit.team_id) ;
    player_id   = std::move(unit.player_id) ;
    unit_vis    = std::move(unit.unit_vis) ;
    dir 		= std::move(unit.dir) ;
    pos 		= std::move(unit.pos) ;
    return *this;
}

// Equals operator
bool CUnit::operator==(const CUnit& rhs)
{
    return (unit_id == rhs.unit_id)
        && (team_id == rhs.team_id)
        && (player_id == rhs.player_id)
        && (unit_vis == rhs.unit_vis)
        && (dir == rhs.dir)
        && (pos == rhs.pos);
}

// Update the visual representation of the unit
unitVis_c CUnit::updateMyVisual()
{
	// Start at invalid
	setVisual(unitVis_invalid);

	dir_to_vis_map::const_iterator it = get_vis_map_V().find(dir);

	// If found set to new vis
	if( it != get_vis_map_V().end() )
		setVisual(it->second);

	return getVisual();
}

// Set the unit from visual
bool CUnit::setFromVisual( const unitVis_c& vis )
{
	dir_to_vis_map::const_iterator it;

	for( it = get_vis_map_V().begin(); it != get_vis_map_V().end(); it++ )
	{
		if( it->second == vis )
		{
            dir = it->first;
			updateMyVisual();
			return true;
		}
	}

	// No matching direction to visual
	return false;
}

// Turn unit left
dir_t CUnit::turnLeft()
{
    switch( dir )
    {
    case dir_t::N:
        dir = dir_t::W;
        break;

    case dir_t::E:
        dir = dir_t::N;
        break;

    case dir_t::S:
        dir = dir_t::E;
        break;

    case dir_t::W:
        dir = dir_t::S;
        break;
    }

    updateMyVisual();

    return getDir();
}

// Turn unit right
dir_t CUnit::turnRight()
{
    switch( dir )
    {
    case dir_t::N:
        dir = dir_t::E;
        break;

    case dir_t::E:
        dir = dir_t::S;
        break;

    case dir_t::S:
        dir = dir_t::W;
        break;

    case dir_t::W:
        dir = dir_t::N;
        break;
    }

    updateMyVisual();

    return getDir();
}

// Turn unit around
dir_t CUnit::turnAround()
{
    switch( dir )
    {
    case dir_t::N:
        dir = dir_t::S;
        break;

    case dir_t::E:
        dir = dir_t::W;
        break;

    case dir_t::S:
        dir = dir_t::N;
        break;

    case dir_t::W:
        dir = dir_t::E;
        break;
    }

    updateMyVisual();

    return getDir();
}

// Get the co-ordinate infront of the unit
uvector2 CUnit::getInFront() const
{
    vector2 delta = vecFromDir(dir);
    return pos + delta;
}
