#include "unitv.h"

#include <map> // for std::map

CUnitV::CUnitV()
: dir(dir_t::S)
{

}


// Map of visual representation of unitv
static const std::map< dir_t, unitVis_c > sk_visMap =
{
	{dir_t::N,'^'},
	{dir_t::E,'>'},
	{dir_t::S,'v'},
	{dir_t::W,'<'},
};

unitVis_c CUnitV::getVisual() const
{
	std::map< dir_t, char >::const_iterator it = sk_visMap.find(dir);

	if( it == sk_visMap.end() )
	{
		return 0;
	}

	return it->second;
}

bool CUnitV::setFromVisual( unitVis_c& vis )
{
	std::map< dir_t, char >::const_iterator it;

	for( it = sk_visMap.begin(); it != sk_visMap.end(); it++ )
	{
		if( it->second == vis )
		{
			dir == it->first;
			return true;
		}
	}

	// No matching direction to visual
	return false;
}