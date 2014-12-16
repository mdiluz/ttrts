#include "unit.h"

// Unit types
#include "unitv.h"

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