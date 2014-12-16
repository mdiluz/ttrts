#include "unitv.h"

#include <map> // for std::map

CUnitV::CUnitV()
: dir(dir_t::S)
{
	updateMyVisual();
}


// Map of visual representation of unitv
namespace 
{	
	typedef std::map< dir_t, unitVis_c > dir_to_vis_map;
	
	// Helper function to get the vis map during static init
	const dir_to_vis_map& get_vis_map()
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

// Update the visual representation of the unit
unitVis_c CUnitV::updateMyVisual()
{
	// Start at invalid
	setVisual(unitVis_invalid);

	dir_to_vis_map::const_iterator it = get_vis_map().find(dir);

	// If found set to new vis
	if( it != get_vis_map().end() )
		setVisual(it->second);

	return getVisual();
}

bool CUnitV::setFromVisual( unitVis_c& vis )
{
	dir_to_vis_map::const_iterator it;

	for( it = get_vis_map().begin(); it != get_vis_map().end(); it++ )
	{
		if( it->second == vis )
		{
			dir == it->first;
			updateMyVisual();
			return true;
		}
	}

	// No matching direction to visual
	return false;
}