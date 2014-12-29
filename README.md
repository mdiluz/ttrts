# TTRTS v0.3.0
*The Tiny Terminal RTS where the players write their AIs*

-----------------------------------------------------------
## Introduction
A simple terminal based RTS game that uses text files to communicate game state and unit commands. 

TTRTS is from the ground up designed to be a fun way to practice programming. Any programming language than can handle file I/O can be used to make an AI for TTRTS, and this extensibility allows for any type of programmer to enjoy designing and playing against their friends.

-----------------------------------------------------------
## Building TTRTS

#### Requirements
* cmake - our build system uses cmake
* Linux/OSX - currently no support for Windows, tracked with [Issue #9](https://github.com/mdiluz/ttrts/issues/9)

#### To Build
    $ git clone https://github.com/mdiluz/ttrts.git
    $ cd ttrts
    $ ./bootstrap.sh
    $ ./ttrts # To launch binary and display usage
    
-----------------------------------------------------------
## Further Information

See [the ttrts binary readme](source/ttrts/README.md) for full usage and game rules

See [my ttrts-players repository](https://github.com/mdiluz/ttrts-players) for examples of players
