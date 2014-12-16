#ifndef _UNIT_H_
#define _UNIT_H_

#include <string>

#include "vector2.h"

// Type for the unit type-id
typedef char unit_c;

// Base unit type
template < unit_c unit_cype >
class CUnit
{
public:

	CUnit() = default;
	virtual ~CUnit() = default;

private:

	// All units must have position
	uvector2 pos;
};

#endif //_UNIT_H_