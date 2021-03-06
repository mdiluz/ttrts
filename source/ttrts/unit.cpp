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
	typedef std::map<dir_c, unitvis_c> dir_to_vis_map;
	
	// Helper function to get the vis map during static init
	const dir_to_vis_map& get_vis_map_V()
	{
		static const dir_to_vis_map sk_visMap =
		{
			{dir_c::N,'^'},
			{dir_c::E,'>'},
			{dir_c::S,'v'},
			{dir_c::W,'<'},
		};

		return sk_visMap;
	}
}

// force a reset of the unit ID value
void __forceResetCUnitID()
{
    unit_id_t i = 0;
    get_unique_unit_id(&i);
}

// Get a unit from a visual
CUnit CUnit::GetUnitFromVis( unitvis_c vis )
{
    CUnit unit;
    unit.SetFromVisual(vis);
    return unit;
}

// Plain constructor
CUnit::CUnit()
: unit_id 	( get_unique_unit_id() )
, player_id ( player_t::NUM_INVALID )
, unit_vis 	(unitvis_invalid)
, dir 		( dir_c::S )
, pos 		( { ucoord_invalid, ucoord_invalid } )
{
    UpdateMyVisual();
}

// Move constructor
CUnit::CUnit(CUnit&& unit)
: unit_id 	( std::move(unit.unit_id) )
, player_id ( std::move(unit.player_id) )
, unit_vis 	( std::move(unit.unit_vis) )
, dir 		( std::move(unit.dir) )
, pos 		( std::move(unit.pos) )
{
    UpdateMyVisual();
}


// Move asignment operator
CUnit& CUnit::operator=(CUnit&& unit)
{
    unit_id 	= std::move(unit.unit_id) ;
    player_id 	= std::move(unit.player_id) ;
    unit_vis    = std::move(unit.unit_vis) ;
    dir 		= std::move(unit.dir) ;
    pos 		= std::move(unit.pos) ;
    return *this;
}

// Equals operator
bool CUnit::operator==(const CUnit& rhs)
{
    return (unit_id == rhs.unit_id)
        && (player_id == rhs.player_id)
        && (unit_vis == rhs.unit_vis)
        && (dir == rhs.dir)
        && (pos == rhs.pos);
}

// Update the visual representation of the unit
unitvis_c CUnit::UpdateMyVisual()
{
	// Start at invalid
    SetVisual(unitvis_invalid);

	dir_to_vis_map::const_iterator it = get_vis_map_V().find(dir);

	// If found set to new vis
	if( it != get_vis_map_V().end() )
        SetVisual(it->second);

	return GetVisual();
}

// Set the unit from visual
bool CUnit::SetFromVisual(const unitvis_c &vis)
{
	dir_to_vis_map::const_iterator it;

	for( it = get_vis_map_V().begin(); it != get_vis_map_V().end(); it++ )
	{
		if( it->second == vis )
		{
            dir = it->first;
            UpdateMyVisual();
			return true;
		}
	}

	// No matching direction to visual
	return false;
}

// Turn unit left
dir_c CUnit::TurnLeft()
{
    switch( dir )
    {
    case dir_c::N:
        dir = dir_c::W;
        break;

    case dir_c::E:
        dir = dir_c::N;
        break;

    case dir_c::S:
        dir = dir_c::E;
        break;

    case dir_c::W:
        dir = dir_c::S;
        break;
    }

    UpdateMyVisual();

    return GetDir();
}

// Turn unit right
dir_c CUnit::TurnRight()
{
    switch( dir )
    {
    case dir_c::N:
        dir = dir_c::E;
        break;

    case dir_c::E:
        dir = dir_c::S;
        break;

    case dir_c::S:
        dir = dir_c::W;
        break;

    case dir_c::W:
        dir = dir_c::N;
        break;
    }

    UpdateMyVisual();

    return GetDir();
}

// Turn unit around
dir_c CUnit::TurnAround()
{
    switch( dir )
    {
    case dir_c::N:
        dir = dir_c::S;
        break;

    case dir_c::E:
        dir = dir_c::W;
        break;

    case dir_c::S:
        dir = dir_c::N;
        break;

    case dir_c::W:
        dir = dir_c::E;
        break;
    }

    UpdateMyVisual();

    return GetDir();
}

// Get the co-ordinate infront of the unit
uvector2 CUnit::GetInFront() const
{
    vector2 delta = vecFromDir(dir);
    return pos + delta;
}
