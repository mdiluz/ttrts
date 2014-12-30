# NAME
	ttrts - Tiny Terminal RTS

# SYNOPSIS
	ttrts MAPFILE

# DESCRIPTION
	ttrts is a tiny terminal based RTS that uses text files as order lists to control the units

	This means that any user, program or cat that can read and write to text files can play the game

# RETURN VALUE
    ttrts will return -1 on error, or the winning player on completion

# OPTIONS
	MAPFILE - File to read in the initial game state. Local or in ${TTRTS_MAPS}

# USAGE
	When invoked, ttrts will set up the game in a directory within ${TTRTS_GAMES} by the name of the map

	The files in this directory can be read and interpreted by human, robot or cat

	ttrts will then await order files from each participant

	Once all order files have been received ttrts will calculate the turn and output a new gamestate file

	This process repeats until the game is over

# ENVIRONMENT
	${TTRTS_MAPS}  - Map file lookup location, defaults to `/usr/share/ttrts/maps/`

	${TTRTS_GAMES} - Game directory for I/O, defaults to `/tmp/`

-----------------------------------------------------------
# FILES
    `/usr/share/ttrts/maps/` holds a sample set of maps

## Gamestate File
	Turn_{TURNNUMBER}.txt

### Contents
    ===== ttrts v{MAJOR}.{MINOR}.{PATCH} =====
    NAME:{GAMENAME}
    SIZE:[{X},{Y}]
    TURN:{TURNNUMBER}
    WALL:[{X},{Y}][{X},{Y}][{X},{Y}]...{repeat for all walls}
    ~~~~
    UNIT:{ID} pl:{PLAYER} vs:{VIS} dr:{DIR(NESW)} ps:[{X},{Y}]
    ... {continue for all units}
    END

## Order File
    Player_{PLAYER_ID}_Turn_{TURN_NUMBER}.txt

### Contents
    ORDER:{ORDER_CHAR} id:{UNIT_ID}
    ... {continue for all orders}
    END

-----------------------------------------------------------
# GAMEPLAY

	The game takes place in a series of simultaneous turns on an arbitrarily sized 2D board

	Each turn, the client outputs a gamestate file and waits for an order file from each player

	All commands are evaluated simultaneously with friendly fire enabled by default

	The game is over when any of three conditions are met -
    * All remaining units are controlled by a single player
    * No units are left (draw)
    * All units left are unable to move (draw)

# UNITS
	Each unit occupies a single tile on the board, facing in a compass direction (NESW)

	Units will only accept orders from their owner

	Units can receive only a single order each turn

	Units cannot occupy the same tile as other units/walls

# ORDERS
### F   - Move unit [F]orward one space, leaving a wall

	This wall will remain until the end of the game, blocking movement to that tile

	Movement orders have no effect if impossible, eg.
    * Attempting to move outside of map
    * Attempting to move on to tile occupied by unit/wall

### L/R - Rotate unit [L]eft or [R]ight

	Unit will rotate clockwise or counter-clockwise, this order cannot fail

### A   - [A]ttack in straight line in front of unit

	Attack will continue forward until unit can't progress, all units within the path of the attack are destroyed.
