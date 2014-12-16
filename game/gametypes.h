#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include <limits>       // std::numeric_limits

// Type for a team IDs
enum class Team : char
{
    Red = 0,
    Blue,
    Green,
    Yellow,
    NUM_INVALID
};

// Type for player IDs
typedef unsigned char player_id_t;

// Type for unit IDs
typedef unsigned short unit_id_t;

// Typedef for unit visual representations
typedef char unitVis_c;

// Invalid data for above types
static const player_id_t 	player_id_invalid 	= std::numeric_limits<player_id_t>::max();
static const unit_id_t 		unit_id_invalid 	= std::numeric_limits<unit_id_t>::max();
static const unitVis_c 		unitVis_invalid 	= std::numeric_limits<unitVis_c>::max();

#endif //_GAME_TYPES_H_