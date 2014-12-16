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

// Get a unit from a visual
CUnit CUnit::getUnitFromVis( unitVis_c vis )
{
	CUnit unit;
	unit.setFromVisual(vis);
    return unit;
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
