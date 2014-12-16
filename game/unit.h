#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>
#include <memory>

#include "gametypes.h"
#include "vector2.h"

// Base unit type
class CUnit
{
public:
	virtual ~CUnit() = default;


	inline const unit_id_t& 	getID() const 		{ return unit_id; }
	inline const team_id_t& 	getTeam() const 	{ return team_id; }
	inline const player_id_t& 	getPlayer() const 	{ return player_id; }
	inline const unitVis_c&		getVisual() const 	{ return unit_vis; }

	// Return non-zero values on error
	inline int 	setTeam(const team_id_t& v)  		{ return (v == team_id_invalid) ? 	-1 : (( team_id = v ), 0); }
	inline int 	setPlayer(const player_id_t& v)  	{ return (v == player_id_invalid) ? -1 : (( player_id = v ), 0); }
	inline int	setVisual(const unitVis_c& v)  		{ return (v == unitVis_invalid) ? 	-1 : (( unit_vis = v ), 0); }

	inline const uvector2& 		getPos() const 						{ return pos; }
	inline void 				setPos(const uvector2& v)  			{ pos = v; }

	// Check unit is valid
	inline bool valid() const;

	// Set a unit based solely on it's visual
	// Maybe make non-virtual at some point to avoid vtable lookups
	virtual bool 		setFromVisual(unitVis_c& vis) = 0;

	// Factory function for creating units from a visual
	static std::unique_ptr<CUnit> getUnitFromVis( unitVis_c vis );

protected:
	
	// Protected constructor, cannot be constructed as base type
	CUnit();

private:

	// Unit ID
	const unit_id_t unit_id;

	// Visual 
	unitVis_c unit_vis;

	// Team ID
	team_id_t team_id;

	// Owner ID
	player_id_t player_id;

	
	// All units must have position
	uvector2 pos;
};

// Simple validation
inline bool CUnit::valid() const
{
	return (unit_id 	!= unit_id_invalid )
		&& (team_id 	!= team_id_invalid )
		&& (player_id 	!= player_id_invalid)
		&& (unit_vis 	!= unitVis_invalid);
}

#endif //_UNIT_H_