#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include <limits>       // std::numeric_limits
#include "stdlib.h" // for size_t

// Type for a Player IDs
enum class player_t : char
{
    Red = 0,
    Green,
    Yellow,
    Blue,
    NUM_INVALID
};

// Type for unit IDs
typedef unsigned short      unit_id_t;       
static const unit_id_t 		unit_id_invalid 	= std::numeric_limits<unit_id_t>::max();

// Typedef for unit visual representations
typedef char                unitvis_c;       
static const unitvis_c      unitvis_invalid 	= std::numeric_limits<unitvis_c>::max();

// Coordinate types
typedef short 				coord_t;
static const coord_t 		coord_invalid 	= std::numeric_limits<coord_t>::max();

typedef unsigned short 		ucoord_t;
static const ucoord_t 		ucoord_invalid 	= std::numeric_limits<ucoord_t>::max();

// Direction representation
enum class dir_c : char
{
	N = 'N',
	S = 'S',
	E = 'E',
    W = 'W'
};

// Helper function for count of an array
template<class T, size_t N>
constexpr size_t _countof(T (&)[N]) { return N; }

#endif //_GAME_TYPES_H_