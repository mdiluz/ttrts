#ifndef _GAME_TYPES_H_
#define _GAME_TYPES_H_

#include <limits>       // std::numeric_limits

// Type for a team IDs
typedef unsigned short team_id_t;

// Type for player IDs
typedef unsigned short player_id_t;

// Type for unit IDs
typedef unsigned short unit_id_t;

// Type for the unit type-id
typedef char unitType_c;

// Typedef for unit visual representations
typedef char unitVis_c;

// Invalid data for above types
static const team_id_t 		team_id_invalid 	= std::numeric_limits<team_id_t>::max();
static const player_id_t 	player_id_invalid 	= std::numeric_limits<player_id_t>::max();
static const unit_id_t 		unit_id_invalid 	= std::numeric_limits<unit_id_t>::max();
static const unitType_c 	unitType_invalid 	= std::numeric_limits<unitType_c>::max();
static const unitVis_c 		unitVis_invalid 	= std::numeric_limits<unitVis_c>::max();

#endif //_GAME_TYPES_H_