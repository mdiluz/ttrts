ttrts
=====

*the Tiny Terminal RTS where the players write their AIs*

Introduction
------------
We aim to create a simple terminal based rts where the user programs their armies AI.

Targets
-------
### ttrts
Main ttrts executable , runs from the command line and can act as host or client

### ttrts-test
Test executable, to be compiled and run to test various functionality

Libraries
---------
### game
Implementation of the RTS rules and simulation

### net
Net code for hosting the server and communicating with clients

### ui
Wrapper for user interface for the terminal, this only really needs three stages
* Initialise the game with settings and connect the clients
* Run the game simulation to it's conclusion
* Display the game result

### player
Custom player AI code, this should contain examples and test code to help newcomers begin their journey