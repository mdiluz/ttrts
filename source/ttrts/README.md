## NAME
	ttrts - Tiny Terminal RTS

## SYNOPSYS
	ttrts MAPFILE

## DESCRIPTION
	ttrts is a tiny terminal based RTS where that uses text
	files as order lists to control it's units.

	This means that any user, program or cat that can read
	and write to text files can play the game.

## USAGE
	When invoked, ttrts will set up a full game and output a
	single file representing the current gamestate into a 
	local directory called `ttrts_{GAME_NAME}`.

	This file can be read in and interpretted by human, robot
	or cat. ttrts will wait for orders files to be placed in 
	it's current working directory.

	Once orders have been set for each player taking part 
	ttrts will calculate the new game state and output a new 
	gamestate file for the next turn.

	This process repeats until a winner is chosen!

## OPTIONS
	MAPFILE - File to read in the initial game state from

--------------------------------------------------------------

## GAMESTATE FILE FORMAT
### Name
	Turn_{TURN_NUMBER}.txt
### Contents
	===== ttrts v{MAJOR}.{MINOR}.{PATCH} =====
	NAME:{GAMENAME}
	SIZE:[{X},{Y}]
	TURN:{TURN_NUMBER}
	... {any extra properties could go here}
	~~~~
	UNIT:{ID} tm:{TEAM} vs:{VIS} dr:{DIR(NESW)} ps:[{X},{Y}]
	... {continue for all units}
	END

## ORDER FILE FORMAT
### Name
	Turn_{TURN_NUMBER}_Team_{TEAM_NUMBER}.txt
### Contents
	ORDER:{ORDER_CHAR} id:{UNIT_ID}
	... {continue for all orders}
	END

### Orders
	F   - move unit [F]orward one space
	L/R - rotate unit [L]eft or [R]ight
	A   - [A]ttack in straight line in front of unit