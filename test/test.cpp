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
        order.order = order_c::F;
		order.unit = 10;
		std::string order_string = GetStringFromOrder(order);
		COrder order2 = GetOrderFromString(order_string);

		if ( order2 != order )
			return "failed order string conversion test";
	}

	{
		COrder order;
        order.order = order_c::F;
		if (!isMovementOrder(order) )
			return "Failed to detect a movement order";

		if (isActionOrder(order) )
			return "Wrongly detected an action order";
	}

	{
		COrder order;
        order.order = order_c::A;
		if (! isActionOrder(order) )
			return "Failed to detect a action order";

		if (isMovementOrder(order) )
			return "Wrongly detected an movement order";
	}

	{
		CTTRTSGame game( 15, 10 );
		if( game.SimulateToNextTurn() )
			return "Failed to simulate a blank game";

        if( game.GetNumUnits() )
            return "Game started with non-zero unit number";
    }

    {
        CTTRTSGame game( 5, 5 );

        {
            CUnit unit = CUnit::getUnitFromVis('^');
            unit.setPos( {2,2} );
            unit.setPlayer(0);

            game.AddUnit(std::move(unit));
        }

        {
            CUnit unit = CUnit::getUnitFromVis('^');
            unit.setPos( {2,2} );
            unit.setPlayer(0);

            if( !game.AddUnit(std::move(unit)) )
                return "Game should have rejected unit placed on the same spot";

            if( game.GetNumUnits() != 1 )
                return "Game ended up with too many units";
        }
    }

    {
        CTTRTSGame game( 5, 5 );

        CUnit unit = CUnit::getUnitFromVis('>');
        const unit_id_t id = unit.getID();
        COrder order;

        unit.setPos( {2,2} );
        unit.setPlayer(0);

        if ( game.AddUnit(std::move(unit)) )
            return "Game failed to add valid unit";

        order.unit = id;
        order.order = order_c::F;

        if( game.IssueOrder(0,order) )
            return "Game failed to issue valid order";

        if (game.SimulateToNextTurn() )
            return "Game failed to simulate valid turn";

        if( game.GetUnitByIDConst(id).getPos() != uvector2{3,2} )
            return "Simple movement order failed";

    }

    {
        CTTRTSGame game( 2, 1 );

        unit_id_t id;
        {
            CUnit unit = CUnit::getUnitFromVis('>');
            id = unit.getID();
            COrder order;

            unit.setPos( {0,0} );
            unit.setPlayer(0);

            if ( game.AddUnit(std::move(unit)) )
                return "Game failed to add valid unit";

            order.unit = id;
            order.order = order_c::A;

            if( game.IssueOrder(0,order) )
                return "Game failed to issue valid order";
        }
        {
            CUnit unit = CUnit::getUnitFromVis('<');

            unit.setPos( {1,0} );
            unit.setPlayer(1);

            if ( game.AddUnit(std::move(unit)) )
                return "Game failed to add valid unit";
        }

        game.SimulateToNextTurn();

        if ( game.GetNumUnits() != 1 )
            return "Game failed to kill a unit when it logically should have";

        if ( game.GetUnitByIndex(0).getDir() != dir_t::E )
            return "Game killed the wrong unit";

        if ( game.GetUnitByIndex(0).getID() != id )
            return "Game killed the wrong unit";
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
