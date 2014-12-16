#ifndef _BASETYPES_H_
#define _BASETYPES_H_

// Coordinate types
typedef short 			coord_t;
typedef unsigned short 	ucoord_t;

// Direction representation
enum dir_t : char
{
	N = 'N',
	S = 'S',
	E = 'E',
	W = 'W',
};

#endif //_BASETYPES_H_