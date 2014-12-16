#include <string.h>
#include "unit.h"

#include <map> // for std::map

namespace
{
	// Helper function for generating unique unit ids during static init
	unit_id_t get_unique_unit_id(unit_id_t* set = nullptr)
	{
		static unit_id_t p = 0;

        // If we have a set value, then set our int
        if( set )
            p = *set;

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

// force a reset of the unit ID value
void forceResetUnitId()
{
    unit_id_t i = 0;
    get_unique_unit_id(&i);
}

// Get a unit from a visual
CUnit CUnit::GetUnitFromVis( unitVis_c vis )
{
    CUnit unit;
    unit.setFromVisual(vis);
    return unit;
}

// Get a string descriptor of a unit
// "U id:[unit_id] team:[team_id] player:[player_id] vis:[unit_vis] dir:[dir] pos:[pos.x],[pos.y]"
std::string CUnit::GetStringFromUnit(const CUnit& unit )
{
    static char buff[128];
    memset(buff,0,sizeof(buff));

    snprintf(buff,128, UNIT_FORMATTER,
            unit.unit_id,
            (int)unit.team_id,
            unit.unit_vis,
            unit.dir,
            unit.pos.x,
            unit.pos.y );

    return buff;
}

// Get a unit from a string descriptor
// "U id:[unit_id] team:[team_id] player:[player_id] vis:[unit_vis] dir:[dir] pos:[pos.x],[pos.y]"
CUnit CUnit::GetUnitFromString(const std::string& unit )
{
    CUnit ret;

    unsigned int id;
    int team;
    char vis;
    char dir;
    unsigned int posx;
    unsigned int posy;

    sscanf(unit.c_str(), UNIT_FORMATTER,
            &id,
            &team,
            &vis,
            &dir,
            &posx,
            &posy );

    ret.unit_id = (unit_id_t)id;
    ret.team_id = (Team)team;
    ret.unit_vis = (unitVis_c)vis;
    ret.dir = (dir_t)dir;
    ret.pos = uvector2(posx,posy);

    return ret;
}

// Plain constructor
CUnit::CUnit()
: unit_id 	( get_unique_unit_id() )
, team_id 	( Team::NUM_INVALID )
, unit_vis 	( unitVis_invalid )
, dir 		( dir_t::S )
, pos 		( { ucoord_invalid, ucoord_invalid } )
{
	updateMyVisual();
}

// Move constructor
CUnit::CUnit(CUnit&& unit)
: unit_id 	( std::move(unit.unit_id) )
, team_id 	( std::move(unit.team_id) )
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
