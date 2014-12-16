#include "game.h"

#include <iostream>
#include <fstream>

void AddUnitToGame( Team team, char vis, uvector2 vec, CTTRTSGame& game )
{
    CUnit unit = CUnit::GetUnitFromVis(vis);
    unit.setPos( vec );
    unit.setTeam(team);
    game.AddUnit(std::move(unit));
}

void OutputGame( CTTRTSGame&& game )
{
    std::ofstream output;
    output.open (game.GetName() + ".txt");
    output << game.GetStateAsString();
    output.close();

    forceResetUnitId();
}

int main()
{
    // Tiny 2v2 Game
    {
        CTTRTSGame game(7, 5);
        game.SetName("Tiny2v2");

        AddUnitToGame( Team::Blue, '>', uvector2(1, 1), game);
        AddUnitToGame( Team::Blue, '>', uvector2(1, 3), game);

        AddUnitToGame( Team::Red, '<', uvector2(5, 1), game);
        AddUnitToGame( Team::Red, '<', uvector2(5, 3), game);

        OutputGame(std::move(game));
    }

    // Basic 5v5 game
    {
        CTTRTSGame game(21, 11);
        game.SetName("Basic5v5");

        for ( ucoord_t y : { 1,3,5,7,9 } )
            AddUnitToGame( Team::Blue, '>', uvector2(1, y), game);

        for ( ucoord_t y : { 1,3,5,7,9 } )
            AddUnitToGame( Team::Red, '<', uvector2(19, y), game);

        OutputGame(std::move(game));
    }

    // Chess 10v10 game
    {
        CTTRTSGame game(8, 8);
        game.SetName("Chess");

        for ( ucoord_t y : { 0,1,2,3,4,5,6,7 } ) {
            AddUnitToGame(Team::Blue, '>', uvector2(0, y), game);
            AddUnitToGame(Team::Blue, '>', uvector2(1, y), game);
        }

        for ( ucoord_t y : { 0,1,2,3,4,5,6,7 } ) {
            AddUnitToGame(Team::Red, '<', uvector2(6, y), game);
            AddUnitToGame(Team::Red, '<', uvector2(7, y), game);
        }

        OutputGame(std::move(game));
    }
}