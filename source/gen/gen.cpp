#include "game.h"

#include <iostream>
#include <fstream>
#include "formatters.h"

void AddUnitToGame( player_t player, char vis, uvector2 vec, CTTRTSGame& game )
{
    CUnit unit = CUnit::GetUnitFromVis(vis);
    unit.SetPos(vec);
    unit.SetPlayer(player);
    game.AddUnit(std::move(unit));
}

void OutputGame( CTTRTSGame&& game )
{
    std::ofstream output;
    output.open (game.GetName() + ".txt");
    output << GetStringFromGame(game);
    output.close();

    __forceResetCUnitID();
}

int main()
{
    // Tiny 1v1 Game
    //------
    //-G----
    //----R-
    //-G----
    //----R-
    //------
    {
        CTTRTSGame game(6, 6);
        game.SetName("Tiny2Player");

        AddUnitToGame( player_t::Red, '<', uvector2(4, 2), game);
        AddUnitToGame( player_t::Red, '<', uvector2(4, 4), game);
        AddUnitToGame( player_t::Green, '>', uvector2(1, 1), game);
        AddUnitToGame( player_t::Green, '>', uvector2(1, 3), game);

        game.AddWall(uvector2(3,2));
        game.AddWall(uvector2(3,3));

        OutputGame(std::move(game));
    }

    // Basic 1v1 game
    // --------------------
    // -G------------------
    // ------------------R-
    // -G------------------
    // ------------------R-
    // -G------------------
    // ------------------R-
    // -G------------------
    // ------------------R-
    // -G------------------
    // ------------------R-
    // --------------------
    {
        CTTRTSGame game(20, 12);
        game.SetName("Big2Player");

        for ( ucoord_t y : { 2,4,6,8,10 } )
            AddUnitToGame( player_t::Red, '<', uvector2(18, y), game);
        for ( ucoord_t y : { 1,3,5,7,9 } )
            AddUnitToGame( player_t::Green, '>', uvector2(1, y), game);


        OutputGame(std::move(game));
    }

    // Sort of like Chess
    //GG------
    //------RR
    //GG------
    //------RR
    //GG------
    //------RR
    //GG------
    //------RR
    {
        CTTRTSGame game(8, 8);
        game.SetName("Chess");

        for ( ucoord_t y : { 1,3,5,7 } ) {
            AddUnitToGame(player_t::Red, '<', uvector2(6, y), game);
            AddUnitToGame(player_t::Red, '<', uvector2(7, y), game);
        }

        for ( ucoord_t y : { 0,2,4,6 } ) {
            AddUnitToGame(player_t::Green, '>', uvector2(0, y), game);
            AddUnitToGame(player_t::Green, '>', uvector2(1, y), game);
        }

        OutputGame(std::move(game));
    }

    // Medium 4 player game
    //----------
    //----------
    //---GGGG---
    //--R -- B--
    //--R-  -B--
    //--R-  -B--
    //--R -- B--
    //---YYYY---
    //----------
    //----------
    {
        CTTRTSGame game(10, 10);
        game.SetName("Medium4Player");

        for ( ucoord_t y : { 2,3,4,5 } ) {
            AddUnitToGame(player_t::Red, '>', uvector2(2, y), game);
            AddUnitToGame(player_t::Blue, '<', uvector2(7, y), game);
        }

        for ( ucoord_t x : { 2,3,4,5 } ) {
            AddUnitToGame(player_t::Yellow, '^', uvector2(x,7), game);
            AddUnitToGame(player_t::Green, 'v', uvector2(x,2), game);
        }

        // Diagonal walls
        game.AddWall(uvector2(3,3));
        game.AddWall(uvector2(3,6));
        game.AddWall(uvector2(6,3));
        game.AddWall(uvector2(6,6));

        // middle walls
        game.AddWall(uvector2(4,4));
        game.AddWall(uvector2(4,5));
        game.AddWall(uvector2(5,4));
        game.AddWall(uvector2(5,5));

        OutputGame(std::move(game));
    }

    // Medium 3 player game
    //----------
    //--------Y-
    //--------Y-
    //----------
    //-G--------
    //-G--------
    //----------
    //--------R-
    //--------R-
    //----------
    {
        CTTRTSGame game(10, 10);
        game.SetName("Medium3Player");

        AddUnitToGame(player_t::Red, '<', uvector2(8, 1), game);
        AddUnitToGame(player_t::Red, '<', uvector2(8, 2), game);
        AddUnitToGame(player_t::Green, '>', uvector2(1, 4), game);
        AddUnitToGame(player_t::Green, '>', uvector2(1, 5), game);
        AddUnitToGame(player_t::Yellow, '<', uvector2(8,7), game);
        AddUnitToGame(player_t::Yellow, '<', uvector2(8,8), game);

        OutputGame(std::move(game));
    }
}