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

void OutputGame( CTTRTSGame& game )
{
    std::ofstream output;
    output.open (game.GetName() + ".txt");
    output << game.GetStateAsString();
    output.close();
}

int main()
{
    // Basic 5v5 game
    {
        CTTRTSGame game(21, 11);
        game.SetName("Basic_5_v_5");

        AddUnitToGame( Team::Blue, '>', uvector2(1, 1), game);
        AddUnitToGame( Team::Blue, '>', uvector2(1, 3), game);
        AddUnitToGame( Team::Blue, '>', uvector2(1, 5), game);
        AddUnitToGame( Team::Blue, '>', uvector2(1, 7), game);
        AddUnitToGame( Team::Blue, '>', uvector2(1, 9), game);

        AddUnitToGame( Team::Red, '<', uvector2(19, 1), game);
        AddUnitToGame( Team::Red, '<', uvector2(19, 3), game);
        AddUnitToGame( Team::Red, '<', uvector2(19, 5), game);
        AddUnitToGame( Team::Red, '<', uvector2(19, 7), game);
        AddUnitToGame( Team::Red, '<', uvector2(19, 9), game);

        OutputGame(game);
    }
}