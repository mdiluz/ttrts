#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>
#include <vector>

#include "gametypes.h"
#include "vector2.h"

// force a reset of the unit ID value
void __forceResetCUnitID();

// Base unit type
class CUnit
{
public:

	// Factory function for creating units from a visual
	static CUnit 		GetUnitFromVis( unitvis_c vis );

	// Unit <--> string conversion functions
	static std::string 	GetStringFromUnit(const CUnit& unit );
	static CUnit 		GetUnitFromString(const std::string& unit );

    // Constructor
	CUnit();

    // Move constructor and move assignment. CUnit cannot be copied
    CUnit(CUnit&& unit);
    CUnit& 				operator=(CUnit&& unit);

	bool 				operator==(const CUnit& rhs);
	bool 				operator!=(const CUnit& rhs) { return !(*this == rhs); }

    // Getters for all the members
	inline const unit_id_t&		GetID() const 		{ return unit_id; }
	inline const player_t &		GetPlayer() const 	{ return player_id; }
	inline const unitvis_c &	GetVisual() const 	{ return unit_vis; }
    inline const dir_c &		GetDir() const      { return dir; }
	inline const uvector2&		GetPos() const 						{ return pos; }

	// Set
    inline player_t	 			SetPlayer(const player_t &v)  	{ return (player_id = v); }
    inline unitvis_c 			SetVisual(const unitvis_c &v)  	{ return (unit_vis = v); }
    inline dir_c 				SetDir(const dir_c &v)          { return (dir = v); }
	inline void 				SetPos(const uvector2 &v)  		{ pos = v; }

    // Get the co-ordinate in front of the unit
    uvector2 					GetInFront() const;

	// Check unit is valid
	inline bool 				Valid() const;

	// Set a unit based solely on it's visual
	bool 						SetFromVisual(const unitvis_c &vis);

    // Orientation methods
	dir_c 						TurnLeft();
    dir_c 						TurnRight();
    dir_c 						TurnAround();

private:

    // Update my visual must be called when setting direction
	unitvis_c 				UpdateMyVisual();

	// Unit ID
    unit_id_t 				unit_id;

	// Visual 
	unitvis_c 				unit_vis;

	// Player ID
	player_t 				player_id;

	// Direction
	dir_c 					dir;
	
	// Position
	uvector2 				pos;
};

// Typedef for a vector of units
typedef std::vector< CUnit > 		CUnitVector;
typedef std::vector< unit_id_t > 	CUnitIDVector;

// Simple validation
inline bool CUnit::Valid() const
{
	return (unit_id 	!= unit_id_invalid )
		&& (player_id 	!= player_t::NUM_INVALID )
		&& (unit_vis 	!= unitvis_invalid);
}

#endif //_UNIT_H_
