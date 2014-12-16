#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>
#include <vector>

#include "gametypes.h"
#include "vector2.h"

// Base unit type
class CUnit
{
public:

    // Constructor
	CUnit();

    // Move constructor and move asignement. CUnit cannot be copied
    CUnit(CUnit&& unit);
    CUnit& operator=(CUnit&& unit);

    // Default dtor
	~CUnit() = default;

    // Getters for all the members
	inline const unit_id_t& 	getID() const 		{ return unit_id; }
	inline const team_id_t& 	getTeam() const 	{ return team_id; }
	inline const player_id_t& 	getPlayer() const 	{ return player_id; }
	inline const unitVis_c&		getVisual() const 	{ return unit_vis; }
    inline const dir_t&         getDir() const      { return dir; }

	// Return non-zero values on error
    inline int 	 setTeam(const team_id_t& v)  		{ return (v == team_id_invalid) ? 	-1 : (( team_id = v ), 0); }
    inline int 	 setPlayer(const player_id_t& v)  	{ return (v == player_id_invalid) ? -1 : (( player_id = v ), 0); }
    inline int	 setVisual(const unitVis_c& v)  	{ return (v == unitVis_invalid) ? 	-1 : (( unit_vis = v ), 0); }

    // Set unit direction
    inline dir_t setDir(const dir_t& v)             { return (dir = v); }

	inline const uvector2& 		getPos() const 						{ return pos; }
	inline void 				setPos(const uvector2& v)  			{ pos = v; }

    // Get the co-ordinate infront of the unit
    uvector2 getInFront() const;

	// Check unit is valid
	inline bool valid() const;

	// Set a unit based solely on it's visual
	bool 		setFromVisual( const unitVis_c& vis);

	// Factory function for creating units from a visual
    static CUnit getUnitFromVis( unitVis_c vis );

    // Orientation methods
    dir_t turnLeft();
    dir_t turnRight();
    dir_t turnAround();

private:

    // Update my visual must be called when setting direction
    unitVis_c 	updateMyVisual();

	// Unit ID
    unit_id_t unit_id;

	// Visual 
	unitVis_c unit_vis;

	// Team ID
	team_id_t team_id;

	// Owner ID
	player_id_t player_id;

	// Direction
	dir_t dir;
	
	// Position
	uvector2 pos;
};

// Typedef for a vector of units
typedef std::vector< CUnit > CUnitVector;

// Simple validation
inline bool CUnit::valid() const
{
	return (unit_id 	!= unit_id_invalid )
		&& (team_id 	!= team_id_invalid )
		&& (player_id 	!= player_id_invalid)
		&& (unit_vis 	!= unitVis_invalid);
}

#endif //_UNIT_H_
