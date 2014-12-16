#include <iostream>     // std::cout

#include "unitv.h"
#include "board.h"
#include "orders.h"

const char* tests()
{
	{
		CBoard board = CBoard(10,5);
		board.clear();
		board.fill(48);
	}

	{
		CUnitV myV;
		if( myV.getVisual() != 'v' && myV.getVisual() != '<' && myV.getVisual() != '^' && myV.getVisual() != '>' )
			 return "failed to properly create V unit";
	}

	{
		std::unique_ptr<CUnit> myV = CUnit::getUnitFromVis('v');
		if( myV->getVisual() != 'v' )
			return "failed to properly create V unit";
	}

	{
		COrder order;
		order.order = 'F';
		order.unit = 10;
		std::string order_string = GetStringFromOrder(order);
		COrder order2 = GetOrderFromString(order_string);

		if ( order2 != order )
			return "failed order string conversion test";
	}

	{
		COrder order;
		order.order = 'F';
		if (!isMovementOrder(order) )
			return "Failed to detect a movement order";

		if (isActionOrder(order) )
			return "Wrongly detected an action order";
	}

	{
		COrder order;
		order.order = 'L';
		if (! isActionOrder(order) )
			return "Failed to detect a action order";

		if (isMovementOrder(order) )
			return "Wrongly detected an movement order";
	}

	return nullptr;
}

// Main program entry point
int main()
{
	std::cout<<"Running tests"<<std::endl;

	const char* res = tests();

	if( res )
	{
		std::cout<<"Tests failed - "<<res<<std::endl;
		return -1;
	}

	std::cout<<"Tests succeeded"<<std::endl;
	return 0;
};