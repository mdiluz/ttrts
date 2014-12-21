#include "game.h"

#include <iostream>
#include <fstream>

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
    output << game.GetStateAsString();
    output.close();

    __forceResetCUnitID();
}

int main()
{
    // Tiny 2v2 Game
    {
        CTTRTSGame game(6, 6);
        game.SetName("Tiny2v2");

        AddUnitToGame( player_t::Red, '<', uvector2(4, 2), game);
        AddUnitToGame( player_t::Red, '<', uvector2(4, 4), game);
        AddUnitToGame( player_t::Green, '>', uvector2(1, 1), game);
        AddUnitToGame( player_t::Green, '>', uvector2(1, 3), game);

        OutputGame(std::move(game));
    }

    // Basic 5v5 game
    {
        CTTRTSGame game(20, 12);
        game.SetName("Big2v2");

        for ( ucoord_t y : { 2,4,6,8,10 } )
            AddUnitToGame( player_t::Red, '<', uvector2(18, y), game);
        for ( ucoord_t y : { 1,3,5,7,9 } )
            AddUnitToGame( player_t::Green, '>', uvector2(1, y), game);


        OutputGame(std::move(game));
    }

    // Chess 10v10 game
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
}