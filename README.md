# TTRTS
*The Tiny Terminal RTS where the players write their AIs*

------------------------------------------------------------------------------------
## Introduction
We aim to create a simple terminal based rts where a user can program an AI to control their army

------------------------------------------------------------------------------------
## Gameplay
1. ttrts clients are run from the command line
2. ttrts-server is launched from the command line 
3. clients will connect to server and confirm initial board state
 1. clients output a text file with game data for this turn
 2. a player, or program, reads the game data file and outputs an instructions file
 3. clients read the instructions file, simulates the turn 
 4. game state is verified between clients and server
 5. repeat until an end state is reached
4. once game is finished, host and clients disconnect and a winner is notified

*see [game](game) for full game rules*

------------------------------------------------------------------------------------
## Source


### Targets
##### ttrts
Main TTRTS executable , runs from the command line and acts as client

##### ttrts-server
TTRTS server executable, runs from the command line acting as server

##### player
Custom player AI code, this should contain examples and test code to help newcomers begin their journey

##### ttrts-test
Test executable, to be compiled and run to test various functionality

### Libraries
##### game
Implementation of the RTS rules and simulation. [game](game) has full information on it's implementation.

##### net
Net code for hosting the server and communicating with clients

##### ui
Wrapper for user interface for the terminal, this only really needs three stages
* Initialise the game with settings and connect the clients
* Run the game simulation to it's conclusion
* Display the game result
* ASCII Colour wrapper for separate teams 

##### maths
simple maths library for 2D calculations and types

