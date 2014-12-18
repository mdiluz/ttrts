# Targets
### ttrts
Main TTRTS executable , runs from the command line and acts as client

### ttrts-test
Test executable, to be compiled and run to test various functionality

### ttrts-gen
Binary to generate map example map files

# Libraries
### game
Implementation of the RTS rules and simulation. [game](game) has full information on it's implementation.

### net
Net code for hosting the server and communicating with clients

### ui
Wrapper for user interface for the terminal, this only really needs three stages
* Initialise the game with settings and connect the clients
* Run the game simulation to it's conclusion
* Display the game result
* ASCII Colour wrapper for separate teams 

### maths
simple maths library for 2D calculations and types

