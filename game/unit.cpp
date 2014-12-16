#include "unit.h"

// Unit types
#include "unitv.h"

namespace
{	
	// Helper function for generating unique unit ids during static init
	unit_id_t get_unique_unit_id()
	{
		static unit_id_t p = 0;
		return p++;
	}
}

CUnit::CUnit()
: unit_id ( get_unique_unit_id() )
, team_id ( team_id_invalid )
, player_id ( player_id_invalid )
, unit_vis ( unitVis_invalid )
{

};

std::unique_ptr<CUnit> CUnit::getUnitFromVis( unitVis_c vis )
{
	switch( vis )
	{
		// Match with any image for a V
		case '^':
		case '>':
		case 'v':
		case '<':
		{
			// Create a V
			std::unique_ptr<CUnit> p = std::unique_ptr<CUnit>(new CUnitV);
			if( (bool)p && p->setFromVisual(vis) )
			{
				return std::move(p);
			}
			break;
		}
	}

	// No unit found, return nullptr
	return std::move(std::unique_ptr<CUnit>(nullptr));
}