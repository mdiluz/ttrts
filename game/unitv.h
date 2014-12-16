#ifndef _UNITV_H_
#define _UNITV_H_

#include "unit.h"

// V unit
class CUnitV
: public CUnit
{
public:
	CUnitV();
	virtual ~CUnitV() = default;

	// Set from a visual
	virtual bool setFromVisual( unitVis_c& vis ) override;

private:

	// Update the visual of V
	unitVis_c 	updateMyVisual();

	// V also has a direction
	dir_t dir;
};

#endif //_UNITV_H_