#include "unit.h"

#include "unitv.h"

#include <memory>

std::unique_ptr<CUnit> CUnit::getUnitFromVis( unitVis_c vis )
{
	switch( vis )
	{
		case '^':
		case '>':
		case 'v':
		case '<':
		{
			std::unique_ptr<CUnit> p = std::unique_ptr<CUnit>(new CUnitV);
			if( (bool)p && p->setFromVisual(vis) )
			{
				return std::move(p);
			}
			break;
		}
	}

	return std::move(std::unique_ptr<CUnit>(nullptr));
}