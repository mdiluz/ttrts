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

	virtual unitVis_c 	getVisual() const = 0;
	virtual bool 		setFromVisual(unitVis_c& vis) = 0;

	static std::unique_ptr<CUnit> getUnitFromVis( unitVis_c vis );

	inline unit_id_t getID() const { return id; }

protected:
	CUnit() = default;

private:

	// Unit ID
	unit_id_t id;

	// Team ID
	team_id_t team_id;

	// Owner ID
	player_id_t owner_id;
	
	// All units must have position
	uvector2 pos;
};

#endif //_UNIT_H_