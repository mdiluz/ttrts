#ifndef _UNITV_H_
#define _UNITV_H_

// V unit
class CUnitV
: public CUnit<'V'>
{
public:
	CUnitV() = default;
	virtual ~CUnitV() = default;

	virtual std::string&& getDescriptor();

private:

	// V also has a direction
	char dir;
};

#endif