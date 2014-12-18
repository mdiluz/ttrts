# TTRTS
*The Tiny Terminal RTS where the players write their AIs*

-------------------------------------------------------------------------------
## Introduction
A simple terminal based RTS game that uses txt files to communicate game state and unit commands. TTRTS was 

-------------------------------------------------------------------------------
## Gameplay
1. The ttrts client is run from the command line with initial parameters
2. The client outputs a gamestate text file 
3. A player, program or cat reads the state and outputs instructions for their units.
4. The client reads in instructions and processes the turn
5. If no winner is reached, skip back to step 2
6. The game client outputs a final summary file with the winner

*see [game](source/game) for full game rules*
