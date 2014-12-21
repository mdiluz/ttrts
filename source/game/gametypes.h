#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include <limits>       // std::numeric_limits

// Type for a Player IDs
enum class Player : char
{
    Red = 0,
    Green,
    Yellow,
    Blue,
    NUM_INVALID
};


typedef unsigned short      unit_id_t;       // Type for unit IDs
typedef char                unitvis_c;       // Typedef for unit visual representations

static const unit_id_t 		unit_id_invalid 	= std::numeric_limits<unit_id_t>::max();
static const unitvis_c      unitvis_invalid = std::numeric_limits<unitvis_c>::max();

#endif //_GAME_TYPES_H_