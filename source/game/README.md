Game Design
=================

The game takes place in a series of simultaneous turns on an arbitrarily sized 2D board.

Each player is in control of a set number of starting units, each turn receives data on the status of the board.

Each player must then issue a single command to each unit in their control.

The engine then takes all commands, evaluates all movement first simultaneously, then all other commands.

All attempted movement to the same square by two or more units will fail.

Friendly fire is enabled by default

--------------------------------------------------------

Units
-----

Currently only one unit, this will be expanded in the future.

Units have a set of properties, and commands than can be issued.

All units take one hit to kill.

##### properties
See [the unit header](unit.h) for full details on unit properties

##### orders
Commands take the form of a single char literal.
See [the order header](order.h) for details on the orders
