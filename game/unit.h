#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>
#include <memory>

#include "vector2.h"

// Type for the unit type-id
typedef char unitType_c;

// Typedef for unit visual representations
typedef char unitVis_c;

// Base unit type
class CUnit
{
public:
	virtual ~CUnit() = default;

	virtual unitVis_c 	getVisual() const = 0;
	virtual bool 		setFromVisual(unitVis_c& vis) = 0;

	static std::unique_ptr<CUnit> getUnitFromVis( unitVis_c vis );

protected:
	CUnit() = default;

private:

	// All units must have position
	uvector2 pos;
};

#endif //_UNIT_H_