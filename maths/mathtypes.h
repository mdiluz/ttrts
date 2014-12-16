#ifndef _BASETYPES_H_
#define _BASETYPES_H_

#include <limits>       // std::numeric_limits

// Coordinate types
typedef short 			coord_t;
typedef unsigned short 	ucoord_t;

// Invalid values
static const coord_t 		coord_invalid 	= std::numeric_limits<coord_t>::max();
static const ucoord_t 		ucoord_invalid 	= std::numeric_limits<ucoord_t>::max();

// Direction representation
enum dir_t : char
{
	N = 'N',
	S = 'S',
	E = 'E',
	W = 'W',
};

#endif //_BASETYPES_H_