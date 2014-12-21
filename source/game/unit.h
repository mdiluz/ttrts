#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>
#include <vector>

#include "gametypes.h"
#include "vector2.h"

#define UNIT_FORMATTER "UNIT:%u tm:%u vs:%c dr:%c ps:[%u,%u]"

// force a reset of the unit ID value
void __forceResetCUnitID();

// Base unit type
class CUnit
{
public:

	// Factory function for creating units from a visual
	static CUnit 		GetUnitFromVis( unitVis_c vis );

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
	inline const Team &			GetTeam() const 	{ return team_id; }
	inline const unitVis_c&		GetVisual() const 	{ return unit_vis; }
    inline const dir_t&			GetDir() const      { return dir; }
	inline const uvector2&		GetPos() const 						{ return pos; }

	// Set
    inline Team 				SetTeam(const Team &v)  		{ return (team_id = v); }
    inline unitVis_c	 		SetVisual(const unitVis_c &v)  	{ return ( unit_vis = v ); }
    inline dir_t 				SetDir(const dir_t &v)          { return (dir = v); }
	inline void 				SetPos(const uvector2 &v)  		{ pos = v; }

    // Get the co-ordinate in front of the unit
    uvector2 					GetInFront() const;

	// Check unit is valid
	inline bool 				Valid() const;

	// Set a unit based solely on it's visual
	bool 						SetFromVisual(const unitVis_c &vis);

    // Orientation methods
    dir_t 						TurnLeft();
    dir_t 						TurnRight();
    dir_t 						TurnAround();

private:

    // Update my visual must be called when setting direction
    unitVis_c 					UpdateMyVisual();

	// Unit ID
    unit_id_t 					unit_id;

	// Visual 
	unitVis_c 					unit_vis;

	// Team ID
	Team 						team_id;

	// Direction
	dir_t 						dir;
	
	// Position
	uvector2 					pos;
};

// Typedef for a vector of units
typedef std::vector< CUnit > 		CUnitVector;
typedef std::vector< unit_id_t > 	CUnitIDVector;

// Simple validation
inline bool CUnit::Valid() const
{
	return (unit_id 	!= unit_id_invalid )
		&& (team_id 	!= Team::NUM_INVALID )
		&& (unit_vis 	!= unitVis_invalid);
}

#endif //_UNIT_H_
