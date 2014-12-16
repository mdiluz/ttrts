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

	virtual unitVis_c 	getVisual() const override;
	virtual bool 		setFromVisual( unitVis_c& vis ) override;

private:

	// V also has a direction
	dir_t dir;
};

#endif //_UNITV_H_