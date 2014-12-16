#ifndef _UNITV_H_
#define _UNITV_H_

#include "unit.h"

// V unit
class CUnitV
: public CUnit<'V'>
{
public:
	CUnitV();
	virtual ~CUnitV() = default;

	virtual unitVis_c getVisual() const;

private:

	// V also has a direction
	dir_t dir;
};

#endif //_UNITV_H_