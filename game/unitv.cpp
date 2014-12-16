#include "unitv.h"

#include <map> // for std::map

CUnitV::CUnitV()
: dir(dir_t::S)
{

}

unitVis_c CUnitV::getVisual() const
{
	// Map of visual representation of unitv
	static const std::map< dir_t, unitVis_c > sk_visMap =
	{
		{dir_t::N,'^'},
		{dir_t::E,'>'},
		{dir_t::S,'v'},
		{dir_t::W,'<'},
	};

	std::map< dir_t, char >::const_iterator it = sk_visMap.find(dir);

	if( it == sk_visMap.end() )
	{
		return 0;
	}

	return it->second;
}