#include "formatters.h"
#include <iostream>

#include "version.h"

// Get the game information as a string
std::string      GetStringFromGame( const CTTRTSGame& game )
{
    // Grab the walls
    std::string walls;
    if( game.GetWalls().size() == 0 )
    {
        walls = "NONE";
    }

    for ( auto wall : game.GetWalls() )
    {
        char pos[16];
        if( snprintf(pos, 16, GAME_POS_FORMATTER , wall.x, wall.y  ) < 0 )
        {
            return "BUFFER OVERFLOW";
        }
        walls += pos;
    }


    // Print out the header
    char header[512];
    if ( snprintf(header, 512, GAME_HEADER_FORMATTER ,
            TTRTS_VERSION_MAJOR,
            TTRTS_VERSION_MINOR,
            TTRTS_VERSION_PATCH,
            game.GetName().c_str(),
            game.GetDimensions().x,
            game.GetDimensions().y,
            game.GetTurn(),
            walls.c_str() ) < 0 )
    {
        return "BUFFER OVERFLOW";
    }

    // Gather unit information
    std::string units;
    for ( const SOrderUnitPair & pair : game.GetOrderUnitPairs() )
    {
        units += GetStringFromUnit(pair.unit);
        units += '\n';
    }

    // Append the header and units
    std::string state(header);
    state += '\n';
    state += GAME_HEADER_DELIMITER;
    state += units;
    state += "END";

    return state;
}

// Get the game information as a string
CTTRTSGame       GetGameFromString( const std::string& input )
{
    size_t headerEnd = input.find(GAME_HEADER_DELIMITER);

    std::string header = input.substr(0, headerEnd);
    std::string units = input.substr(headerEnd + strlen(GAME_HEADER_DELIMITER));

    // Grab information from the header
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    char name[64];
    unsigned int turn;
    unsigned int sizex;
    unsigned int sizey;
    char walls[512];
    if( sscanf(header.c_str(), GAME_HEADER_FORMATTER, &major, &minor, &patch, name, &sizex, &sizey, &turn, walls) != 8 )
    {
        std::cerr<<"Error: Failed to properly read game state from text"<<std::endl;
        return CTTRTSGame(0,0);
    }

    if( major != TTRTS_VERSION_MAJOR
        ||  minor != TTRTS_VERSION_MINOR )
    {
        std::cerr<<"Error: ttrts map/binary version missmatch BINARY:v"<<major<<"."<<minor<<"."<<patch<<" FILE:"<<TTRTS_VERSION_STRING<<std::endl;
    }

    std::vector<uvector2> walls_vector;
    {
        std::string walls_str = walls;
        size_t pos;
        while ( ( pos = walls_str.find(']') ) != std::string::npos )
        {
            std::string pos_string = walls_str.substr(1,pos);

            // Use scanf to extract positions

            unsigned int x;
            unsigned int y;
            if( sscanf(pos_string.c_str(), GAME_POS_FORMATTER, &x, &y ) != 2 )
            {
                return CTTRTSGame(0,0);
            }

            // Erase this coordinate
            walls_str.erase(0,pos+1);

            // Append our list
            walls_vector.push_back({(ucoord_t)x,(ucoord_t)y});
        }
    }

    CTTRTSGame game(sizex,sizey);
    game.SetName(name);
    game.SetTurn(turn);

    // For each line, construct a unit
    {
        size_t pos;
        while ((pos = units.find('\n')) != std::string::npos) {
            std::string unit_string = units.substr(0, pos);
            units.erase(0, pos + 1);
            game.AddUnit(GetUnitFromString(unit_string));
        }
    }

    // Add all walls
    for ( auto wall : walls_vector)
    {
        game.AddWall(wall);
    }

    return game;
}

// Get a string descriptor of a unit
std::string GetStringFromUnit(const CUnit& unit )
{
    static char buff[128];
    memset(buff,0,sizeof(buff));

    snprintf(buff,128, UNIT_FORMATTER,
            unit.GetID(),
            (int)unit.GetPlayer(),
            unit.GetVisual(),
            unit.GetDir(),
            unit.GetPos().x,
            unit.GetPos().y );

    return buff;
}

// Get a unit from a string descriptor
CUnit GetUnitFromString(const std::string& unit )
{
    CUnit ret;

    unsigned int id;
    int player;
    char vis;
    char dir;
    unsigned int posx;
    unsigned int posy;

    sscanf(unit.c_str(), UNIT_FORMATTER,
            &id,
            &player,
            &vis,
            &dir,
            &posx,
            &posy );

    ret.ForceSetID((unit_id_t)id);
    ret.SetPlayer((player_t) player);
    ret.SetVisual((unitvis_c)vis);
    ret.SetDir((dir_c)dir);
    ret.SetPos(uvector2(posx,posy));

    return ret;
}

// Convert an order to a string
std::string GetStringFromOrder(const SOrder & order )
{
    static char buff[128];
    memset(buff,0,sizeof(buff));

    snprintf(buff,128, ORDER_FORMATTER,
            order.command,
            order.unit);

    return buff;
}

// Convert a string to an order
SOrder GetOrderFromString( const std::string& order )
{
    SOrder ret;

    char corder;
    unsigned int unit;

    sscanf(order.c_str(), ORDER_FORMATTER,
            &corder,
            &unit);

    ret.command = (command_c)corder;
    ret.unit = unit;

    return ret;
}