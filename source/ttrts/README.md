## NAME
	ttrts - Tiny Terminal RTS

## SYNOPSYS
	ttrts [OPTIONS...] MAPFILE

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

-------------------------------------------------------------------------------

## GAMESTATE FILE FORMAT
### Name
	Turn_{TURN_NUMBER}.txt
### Contents
	===== {GAME_NAME} =====
	SIZE:[{X},{Y}]
	TURN:{TURN_NUMBER}
	~~~~
	UNIT:{ID} tm:{TEAM} vs:{VIS} dr:{DIR(NESW)} ps:[{X},{Y}]
	...

## ORDER FILE FORMAT
### Name
	Turn_{TURN_NUMBER}_Team_{TEAM_NUMBER}.txt
### Contents
	ORDER:{ORDER_CHAR} id:{UNIT_ID}
	...

### Orders
	F   - Move unit forward one space
	L/R - Rotate unit left or right
	A   - Attack row in front of unit
	
`$ cat README.md | sed 's/^#* //g' | sed 's/\t/\\t/g' | sed ':a;N;$!ba;s/\n/\\n\n/g' | sed 's/^/"/' | sed 's/$/"/' | sed '$ d' > usage.h # To convert this file to c++`