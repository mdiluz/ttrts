#include <iostream>     // std::cout

#include "board.h"
#include "orders.h"
#include "game.h"

const char* tests()
{
	{
		CBoard board = CBoard(10,5);
		board.clear();
		board.fill(48);
	}

	{
		CUnit unit;
		unit.setFromVisual('v');
		if( unit.getVisual() != 118 )
			 return "failed to properly create V unit";
	}

	{
		CUnit unit;
		CUnit unit2;
		if( unit.getID() == unit2.getID() )
			 return "Unit IDs the same";
	}

	{
		CUnit unit = CUnit::getUnitFromVis('v');
		if( unit.getVisual() != 'v' )
			return "failed to properly create V unit with factory";
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

	{
		CTTRTSGame game( 15, 10 );
		if( game.SimulateToNextTurn() )
			return "Failed to simulate a blank game";

		if( game.GetNumOrders() )
			return "Game started with non-zero order number";

		if( game.GetNumUnits() )
			return "Game started with non-zero unit number";
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