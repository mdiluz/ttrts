ttrts Game Design
=================

The game takes place in a series of simultanious turns on an arbitrarilly sized 2D board.

Each player is in control of a set number of starting units, each turn recieves data on the status of the board.

Each player must then issue a single command to each unit in thier control.

The engine then takes all commands, evaluates all movement first simultaniously, then all other commands.

All attempted movement to the same square by two or more units will fail.

--------------------------------------------------------
Units
-----

Currently only one unit, this will be expanded in the future.

Units have a set of properties, and commands than can be issued. 
Commands take the form of a single char literal.
All units have 1 health.


## V
V is your basic slow melee unit. It has very basic data and controls, basically acting like a turtle.
Can be represented based on direction by `<` `^` `>` `v`

##### properties
| property | type    | description                       |
|:---------|:--------|:----------------------------------|
| pos      | char[2] | x,y position on the board         |
| dir      | char    | compass direction unit is facing  |

##### commands
| command  | description                                 |
|:---------|:--------------------------------------------|
| L/R      | turn left/right                             |
| F        | move forward                                |
| A        | deal 1 damage to unit directly in front     |

--------------------------------------------------------
The Board
--------

As an example, let's start with a basic starting `[10,5]` board
````
0000000000
0000000000
0000000000
0000000000
0000000000
````
