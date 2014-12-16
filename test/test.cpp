#include <iostream>     // std::cout

#include "board.h"
#include "order.h"
#include "game.h"

const char* tests()
{
    // Test if we can properly set a unit's visual
	{
		CUnit unit;
		unit.setFromVisual('v');
        if( unit.getVisual() != 'v' )
			 return "failed to properly create V unit";
	}

    // Test unique unit IDs
	{
		CUnit unit;
		CUnit unit2;
		if( unit.getID() == unit2.getID() )
			 return "Unit IDs the same";
	}

    // Test basic invalid unit conversion
    {
        CUnit unit1;

        std::string unit1Desc = GetStringFromUnit(unit1);
        CUnit unit2 = GetUnitFromString(unit1Desc);

        if ( unit1 != unit2 )
            return "Failed to convert an empty unit to string and back";
    }

    // Test custom unit conversion
    {
        CUnit unit1;
        unit1.setFromVisual('v');
        unit1.setPlayer(0);
        unit1.setTeam(Team::Green);
        unit1.setPos( uvector2(5,10) );

        std::string unit1Desc = GetStringFromUnit(unit1);
        CUnit unit2 = GetUnitFromString(unit1Desc);

        if ( unit1 != unit2 )
            return "Failed to convert custom unit to string and back";
    }

    // Test if we can successfully create a unit from a visual
	{
		CUnit unit = GetUnitFromVis('v');
		if( unit.getVisual() != 'v' )
			return "failed to properly create V unit with factory";
	}

    // Test if we can successfully convert orders back and forth
	{
		COrder order;
        order.command = command_c::F;
		order.unit = 10;
		std::string order_string = GetStringFromOrder(order);
		COrder order2 = GetOrderFromString(order_string);

		if ( order2 != order )
			return "failed order string conversion test";
	}

    // Test of the game can logically handle a blank game
	{
		CTTRTSGame game( 15, 10 );
		if( game.SimulateToNextTurn() )
			return "Failed to simulate a blank game";

        if( game.GetNumUnits() )
            return "Game started with non-zero unit number";
    }

    // Test if the game correctly rejects units placed ontop of others
    {
        CTTRTSGame game( 5, 5 );

        {
            CUnit unit = GetUnitFromVis('^');
            unit.setPos( {2,2} );
            unit.setPlayer(0);
            unit.setTeam(Team::Red);

            game.AddUnit(std::move(unit));
        }

        {
            CUnit unit = GetUnitFromVis('^');
            unit.setPos( {2,2} );
            unit.setPlayer(0);
            unit.setTeam(Team::Red);

            if( !game.AddUnit(std::move(unit)) )
                return "Game should have rejected unit placed on the same spot";

            if( game.GetNumUnits() != 1 )
                return "Game ended up with too many units";
        }
    }

    // Test on a small board if a movement command succeeds correctly
    {
        CTTRTSGame game( 5, 5 );

        CUnit unit = GetUnitFromVis('>');
        const unit_id_t id = unit.getID();
        COrder order;

        unit.setPos( {2,2} );
        unit.setPlayer(0);
        unit.setTeam(Team::Red);

        if ( game.AddUnit(std::move(unit)) )
            return "Game failed to add valid unit";

        order.unit = id;
        order.command = command_c::F;

        if( game.IssueOrder(0,order) )
            return "Game failed to issue valid order";

        if (game.SimulateToNextTurn() )
            return "Game failed to simulate valid turn";

        if( game.GetUnitByIDConst(id).getPos() != uvector2{3,2} )
            return "Simple movement order failed";

    }

    // Test on a tiny board, whether a unit can correctly attack another
    {
        CTTRTSGame game( 2, 1 );

        unit_id_t id;
        {
            CUnit unit = GetUnitFromVis('>');
            id = unit.getID();
            COrder order;

            unit.setPos( {0,0} );
            unit.setPlayer(1);
            unit.setTeam(Team::Blue);

            if ( game.AddUnit(std::move(unit)) )
                return "Game failed to add valid unit";

            order.unit = id;
            order.command = command_c::A;

            if( game.IssueOrder(0,order) )
                return "Game failed to issue valid order";
        }
        {
            CUnit unit = GetUnitFromVis('<');

            unit.setPos( {1,0} );
            unit.setPlayer(2);
            unit.setTeam(Team::Red);

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

        if ( game.CheckForWin() != Team::Blue )
            return "Game failed to recognise a win for the right Team";
    }

    return 0;
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
