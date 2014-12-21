#include <iostream>     // std::cout

#include "order.h"
#include "game.h"

const char* tests()
{
    // Test if we can properly set a unit's visual
	{
		CUnit unit;
        unit.SetFromVisual('v');
        if(unit.GetVisual() != 'v' )
			 return "failed to properly create V unit";
	}

    // Test unique unit IDs
	{
		CUnit unit;
		CUnit unit2;
		if(unit.GetID() == unit2.GetID() )
			 return "Unit IDs the same";
	}

    // Test basic invalid unit conversion
    {
        CUnit unit1;

        std::string unit1Desc = CUnit::GetStringFromUnit(unit1);
        CUnit unit2 = CUnit::GetUnitFromString(unit1Desc);

        if ( unit1 != unit2 )
            return "Failed to convert an empty unit to string and back";
    }

    // Test custom unit conversion
    {
        CUnit unit1;
        unit1.SetFromVisual('v');
        unit1.SetPlayer(player_t::Green);
        unit1.SetPos(uvector2(5, 10));

        std::string unit1Desc = CUnit::GetStringFromUnit(unit1);
        CUnit unit2 = CUnit::GetUnitFromString(unit1Desc);

        if ( unit1 != unit2 )
            return "Failed to convert custom unit to string and back";
    }

    // Test if we can successfully create a unit from a visual
	{
		CUnit unit = CUnit::GetUnitFromVis('v');
		if(unit.GetVisual() != 'v' )
			return "failed to properly create V unit with factory";
	}

    // Test if we can successfully convert orders back and forth
	{
		SOrder order;
        order.command = command_c::F;
		order.unit = 10;
		std::string order_string = GetStringFromOrder(order);
		SOrder order2 = GetOrderFromString(order_string);

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
            CUnit unit = CUnit::GetUnitFromVis('^');
            unit.SetPos({2, 2});
            unit.SetPlayer(player_t::Red);

            game.AddUnit(std::move(unit));
        }

        {
            CUnit unit = CUnit::GetUnitFromVis('^');
            unit.SetPos({2, 2});
            unit.SetPlayer(player_t::Red);

            if( !game.AddUnit(std::move(unit)) )
                return "Game should have rejected unit placed on the same spot";

            if( game.GetNumUnits() != 1 )
                return "Game ended up with too many units";
        }
    }

    // Test on a small board if a movement command succeeds correctly
    {
        CTTRTSGame game( 5, 5 );

        CUnit unit = CUnit::GetUnitFromVis('>');
        const unit_id_t id = unit.GetID();
        SOrder order;

        unit.SetPos({2, 2});
        unit.SetPlayer(player_t::Red);

        if ( game.AddUnit(std::move(unit)) )
            return "Game failed to add valid unit";

        order.unit = id;
        order.command = command_c::F;

        if( game.IssueOrder(player_t::Red,order) )
            return "Game failed to issue valid order";

        if (game.SimulateToNextTurn() )
            return "Game failed to simulate valid turn";

        if(game.GetUnitByIDConst(id).GetPos() != uvector2{3,2} )
            return "Simple movement order failed";

    }

    // Test on a tiny board, whether a unit can correctly attack another
    {
        CTTRTSGame game( 2, 1 );
        game.SetName("Test_578");

        unit_id_t id;
        {
            CUnit unit = CUnit::GetUnitFromVis('>');
            id = unit.GetID();
            SOrder order;

            unit.SetPos({0, 0});
            unit.SetPlayer(player_t::Blue);

            if ( game.AddUnit(std::move(unit)) )
                return "Game failed to add valid unit";

            order.unit = id;
            order.command = command_c::A;

            if( game.IssueOrder(player_t::Blue,order) )
                return "Game failed to issue valid order";
        }
        {
            CUnit unit = CUnit::GetUnitFromVis('<');

            unit.SetPos({1, 0});
            unit.SetPlayer(player_t::Red);

            if ( game.AddUnit(std::move(unit)) )
                return "Game failed to add valid unit";
        }

        game.SimulateToNextTurn();

        if ( game.GetNumUnits() != 1 )
            return "Game failed to kill a unit when it logically should have";

        if (game.GetUnitByIndex(0).GetDir() != dir_t::E )
            return "Game killed the wrong unit";

        if (game.GetUnitByIndex(0).GetID() != id )
            return "Game killed the wrong unit";

        if ( game.CheckForWin() != player_t::Blue )
            return "Game failed to recognise a win for the right Player";

        std::string game_string = game.GetStateAsString();
        CTTRTSGame game2 = CTTRTSGame::CreateFromString(game_string);

        std::string game2_string = game2.GetStateAsString();

        // Try matching up the game descriptors
        if( game_string != game2_string )
            return "Generating new game from string failed";

    }

    return 0;
}

// Main program entry point
int main()
{
	const char* res = tests();

	if( res )
	{
		std::cout<<"ERROR: "<<res<<std::endl;
		return -1;
	}

	return 0;
};
