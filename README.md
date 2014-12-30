# TTRTS
*The Tiny Terminal RTS where the players write their AIs*

-----------------------------------------------------------
## Introduction
A simple terminal based RTS game that uses text files to communicate game state and unit commands. 

TTRTS is from the ground up designed to be a fun way to practice programming. Any programming language than can handle file I/O can be used to make an AI for TTRTS, and this extensibility allows for any type of programmer to enjoy designing and playing against their friends.

-----------------------------------------------------------
## Building TTRTS

#### Requirements
* CMake - our build system uses cmake
* Linux/OSX - currently no support for Windows, tracked with [Issue #9](https://github.com/mdiluz/ttrts/issues/9)

#### To Build
    $ git clone https://github.com/mdiluz/ttrts.git
    $ cd ttrts
    $ ./bootstrap.sh
    $ ./ttrts # To launch binary and display usage

-----------------------------------------------------------
## Development

* master branch always stores latest stable release
* master/{hotfix} branches store in progress hotfixes for the stable branch
* dev branch stores in progress development
* dev/{feature} branches store features

-----------------------------------------------------------
## Changelog

#### v0.3.1
* Upgraded install target to repository
  * libttrts static library binary in /usr/local/lib
  * ttrts headers in /usr/local/include/ttrts
  * bash completion into /etc/bash_completion.d/
  * man page into /usr/local/man/man6
  * maps into /usr/share/ttrts/maps
* client now supports env variable configuration
  * TTRTS_MAPS for location of map files, defaults to /usr/share/ttrts/maps
  * TTRTS_GAMES for gameplay directories, defaults to /tmp/
* Map files now have proper header
* NOTE: This version is compatible with v0.3.0, but old generated mapfiles will need the additional header line added

#### v0.3.0
* Additional functionality of walls
  * Walls are noted in gamestate file on new "WALL:[X,Y]..." line
  * Walls are impassable by all movement
* Units leave an impassable wall behind after movement
* Game can now end if no units are able to move
* Various C++ api simplifications
* Integration of perl api from [ttrts-players](https://github.com/mdiluz/ttrts-players)

#### v0.2.0 
* All team references changed to player
    * Order file format changed to Player_#_Turn_#.txt
    * Unit descriptors now use pl: instead of tm:
* Various other C++ api corrections and refactors

#### v0.1.0 
* First playable version of ttrts

-----------------------------------------------------------
## Further Information

See the ttrts binary [readme](source/ttrts/README.md) for full usage and game rules

See [ttrts-players](https://github.com/mdiluz/ttrts-players) for examples of AIs
