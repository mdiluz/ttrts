#include "game.h"

#include <algorithm>
#include <string.h>
#include "formatters.h"

CTTRTSGame::CTTRTSGame( ucoord_t c, ucoord_t r )
: dimensions( c,r )
, turn (0), name ( "Custom_Game" )
{

}

// Move constructor
CTTRTSGame::CTTRTSGame(CTTRTSGame&& game)
: m_OrderUnitPairs(std::move(game.m_OrderUnitPairs))
, dimensions(std::move(game.dimensions))
, turn(std::move(game.turn))
, name(std::move(game.name))
, m_walls(std::move(game.m_walls))
{

}

CTTRTSGame& CTTRTSGame::operator=(CTTRTSGame&& game)
{
    m_OrderUnitPairs = std::move(game.m_OrderUnitPairs);
    dimensions = std::move(game.dimensions);
    turn = std::move(game.turn);
    name = std::move(game.name);
    return *this;
}

// Interpret a string of orders
int CTTRTSGame::IssueOrders( player_t player, const std::string& _orders )
{
	COrderVector orderVector;

    // Copy the const orders into a buffer we can edit
    std::string orders = _orders;

    // Find a line end
	size_t pos;
    while ( (pos = orders.find('\n')) != std::string::npos )
	{
        // Grab the string up to the line end
		const std::string sorder = orders.substr(0, pos);

        // Erase all of string up to and including the line end
		orders.erase(0,pos+1);

        // Create an order from the string and push it back
		SOrder order = GetOrderFromString( sorder );
		orderVector.push_back(order);
	}

    // Call our add order by vector method
	return IssueOrders(player,orderVector);
}

// Issue orders by vector to the game
int CTTRTSGame::IssueOrders( player_t player, const COrderVector& orders )
{
	// verify all the orders
    for ( auto order : orders )
	{
        // If any order returns non-zero, back out
        if ( IssueOrder(player,order) )
			return 1;
	}

	return 0;
}

// Issue a single order
int CTTRTSGame::IssueOrder( player_t player, const SOrder & order )
{
    // Verify the order
	if ( VerifyOrder(player,order) )
			return 1;

    // Get the right unit for the order
    for ( SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        if (pair.unit.GetID() == order.unit )
        {
            pair.order = order;
            return 0;
        }
    }

    // Unit was not found, return 2
    return 2;
}

// Verify a position
int CTTRTSGame::VerifyPosIsValidMovement(uvector2 vec) const
{
    // Simply check if within the bounds of our dimensions for now
    if ( ( vec.x >= dimensions.x )
        || ( vec.y >= dimensions.y ) )
    {
        return 1;
    }

    // Check within our invalid positions
    for ( const uvector2& invalid : m_walls)
    {
        if( vec == invalid )
        {
            return 2;
        }
    }

    return 0;
}


// Get a units new position
uvector2 CTTRTSGame::GetNewPosition( const SOrderUnitPair & pair ) const
{
    // Grab the order
    switch ( pair.order.command)
    {
    // For forward orders, grab in front
    case command_c::F:
        return pair.unit.GetInFront();
    // For all other orders, just grab the old position
    default:
        return pair.unit.GetPos();
    }
}

// Simulate and progress to the next turn
// Returns non-zero if simulation failed
int CTTRTSGame::SimulateToNextTurn()
{
    int error = 0;

    // Attempt all movement orders
    for ( SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        switch (  pair.order.command)
        {
        case command_c::F:
            {
                // Verify new unit position will be on the board
                uvector2 newpos = GetNewPosition(pair);

                // Verify the position is even available
                bool possible = (VerifyPosIsValidMovement(newpos) == 0 );

                if ( possible )
                {
                    // If any unit is in this spot, or moving unit moving to said spot, reject this
                    for ( const SOrderUnitPair & pair2 : m_OrderUnitPairs )
                    {
                        // Skip myself
                        if(pair.unit.GetID() == pair2.unit.GetID() ) continue;

                        if( GetNewPosition(pair2) == newpos )
                        {
                            possible = false;
                            break;
                        }
                    }
                }

                // If the movement is still possible
                if ( possible )
                {
                    // Create a wall at our old position
                    AddWall(pair.unit.GetPos());
                    pair.unit.SetPos(newpos);
                }
            }
            break;
        default:
            break;
        }
    }

    // Turn all units that need turning
    for ( SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        switch (  pair.order.command)
        {
            case command_c::L:
            {
                // Simply turn left
                pair.unit.TurnLeft();
            }
                break;
            case command_c::R:
            {
                // Simply turn right
                pair.unit.TurnRight();
            }
                break;
            default:
                break;
        }
    }

    // Iterate through all charge states
    bool charging = true;
    while(charging)
    {
        // Assume no more charging
        charging = false;
        // Initially move all units
        for ( SOrderUnitPair & pair : m_OrderUnitPairs )
        {
            if ( pair.order.command == command_c::A )
            {
                uvector2 newpos = pair.unit.GetInFront();
                // If move would be within the arena
                if (VerifyPosIsValidMovement(newpos) == 0 )
                {
                    pair.unit.SetPos(newpos);

                    // Unit moved, so more charging needs to be done
                    charging = true;
                }
            }
        }

        std::vector< unit_id_t > toKill; // Vector to store which units to kill

        // Initially move all units to check for pass through
        for ( SOrderUnitPair & pair1 : m_OrderUnitPairs )
        if ( pair1.order.command == command_c::A )
        for ( SOrderUnitPair & pair2 : m_OrderUnitPairs )
        if (pair1.unit.GetID() != pair2.unit.GetID() // Don't check the same units
                && pair2.order.command == command_c::A )
        {
            if( CheckForPassThrough(pair1.unit,pair2.unit) )
            {
                toKill.push_back(pair1.unit.GetID());
                toKill.push_back(pair2.unit.GetID());
            }
        }

        // Kill all units to kill
        KillAll(toKill);
        toKill.clear();

        // Check for all matching spots
        for ( SOrderUnitPair & pair1 : m_OrderUnitPairs )
        for ( SOrderUnitPair & pair2 : m_OrderUnitPairs )
        {
            if(pair1.unit.GetID() == pair2.unit.GetID() ) continue; // Don't check the same units

            if(pair1.unit.GetPos() == pair2.unit.GetPos() )
            {
                if( pair1.order.command == command_c::A )
                {
                    toKill.push_back(pair2.unit.GetID());
                }

                if( pair2.order.command == command_c::A )
                {
                    toKill.push_back(pair1.unit.GetID());
                }
            }
        }

        // Kill all units to kill
        KillAll(toKill);
        toKill.clear();
    }

    // Clear all orders
    for ( SOrderUnitPair & pair : m_OrderUnitPairs )
        pair.order = SOrder();

    // Increment the current turn
    turn++;

    return error;
}


// Kill all units in list
void CTTRTSGame::KillAll( std::vector< unit_id_t >& vec )
{
    // Sort and erase all duplicates
    std::sort( vec.begin(), vec.end() );
    vec.erase( std::unique( vec.begin(), vec.end() ), vec.end() );
    for ( auto id : vec )
    {
        for ( OrderUnitPairVector::iterator it = m_OrderUnitPairs.begin();
              it != m_OrderUnitPairs.end();
              it++ )
        {
            if((*it).unit.GetID() == id )
            {
                // Remove the unit from our alive unit pairs
                m_OrderUnitPairs.erase(it);
                break;

            }
        }
    }
}

// Check if two units passed through each other
bool CTTRTSGame::CheckForPassThrough( const CUnit& one, const CUnit& two )
{
    uvector2 pos1 = one.GetPos();
    uvector2 pos2 = two.GetPos();
    dir_c dir1 = one.GetDir();
    dir_c dir2 = two.GetDir();

    if( pos1.x == pos2.x ) { // Same col
        if (pos1.y == (pos2.y + 1)) {
            if (dir1 == dir_c::N && dir2 == dir_c::S)
                return true;
        }
        else if (pos1.y == (pos2.y - 1)) {
            if (dir1 == dir_c::S && dir2 == dir_c::N)
                return true;
        }
    }
    else if( pos1.y == pos2.y ) { // Same row
        if( pos1.x == (pos2.x+1) ) {
            if( dir1 == dir_c::E && dir2 == dir_c::W )
                return true;
        }
        else if( pos1.x == (pos2.x-1) ) {
            if( dir1 == dir_c::E && dir2 == dir_c::W )
                return true;
        }
    }

    return false;
}

// Add a unit, nonzero return value indicates error
int CTTRTSGame::AddUnit( CUnit&& unit )
{	
	// Verify the unit
	if( !unit.Valid() )
		return 1;

	// Verify if the unit can be placed on the current board
	const uvector2 pos = unit.GetPos();
    if( (pos.x >= dimensions.x) || (pos.y >= dimensions.y) )
		return 2;

    // If any unit's position matches, reject this
    for ( const SOrderUnitPair & pair: m_OrderUnitPairs )
    {
        if(pair.unit.GetPos() == unit.GetPos() )
            return 3;
    }

    // Add the unit with a blank order
    m_OrderUnitPairs.push_back( SOrderUnitPair(std::move(unit), SOrder()) );


	return 0;
}
	
// Add a units, nonzero return value indicates error
int CTTRTSGame::AddUnits( CUnitVector&& units )
{	
	CUnitVector::iterator it;

	for ( it = units.begin(); it != units.end(); it++ )
	{	
		// Attempt the unit add
		if ( AddUnit( std::move(*it) )  )
			return 1;
	}

	// All units added successfully
	return 0;
}

// Verify any order
int CTTRTSGame::VerifyOrder( player_t player, const SOrder & order ) const
{
    int ret = 1;

	// Grab the unit ID
	const unit_id_t unitID = order.unit;

    // Attempt to find the unit
    for ( const SOrderUnitPair & pair : m_OrderUnitPairs )
	{
        // Accept if we have the unit
        if (pair.unit.GetID() == unitID
                && pair.unit.GetPlayer() == player)
		{
            ret = 0;
			break;
		}
	}

	// for now, as long as the unit exists we can attempt the order
    return ret;
}

// Get unit by unit ID
const CUnit& CTTRTSGame::GetUnitByIDConst( unit_id_t id ) const
{
    for ( const SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if (pair.unit.GetID() == id  )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Get an order by unit ID
const SOrder & CTTRTSGame::GetOrderByIDConst( unit_id_t id ) const
{
    for ( const SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if (pair.unit.GetID() == id  )
            return pair.order;
    }

    // Return an invalid unit
    static SOrder invalid_order;
    return invalid_order;
}

// Get unit by unit ID
CUnit& CTTRTSGame::GetUnitByID( unit_id_t id )
{
    for ( SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if (pair.unit.GetID() == id )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Get a vector of the players in the current game
std::vector<player_t> CTTRTSGame::GetPlayers() const
{
    std::vector<player_t> players;
    players.reserve(GetNumUnits());

    // Grab all players
    for ( const SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        players.push_back(pair.unit.GetPlayer());
    }

    // Remove dupes
    std::sort( players.begin(), players.end() );
    players.erase( std::unique( players.begin(), players.end() ), players.end() );

    return players;
}

// Check if we have a win state
player_t CTTRTSGame::GetWinningPlayer() const
{
    // Array of units for each Player
    unsigned int units[(int) player_t::NUM_INVALID];
    memset(units,0,sizeof(units));

    // Count up all the units for each Player
    for ( const SOrderUnitPair & pair : m_OrderUnitPairs )
    {
        const int player = (int) pair.unit.GetPlayer();
        units[player] += 1;
    }

    // Default winning Player to invalid (no win)
    player_t winningPlayer = player_t::NUM_INVALID;

    // For each of the players
    for ( unsigned int i = 0; i < _countof(units); i++ )
    {
        // if there are still units in this Player, and the winning Player hasn't been set
        if( units[i] > 0 && winningPlayer == player_t::NUM_INVALID )
        {
            winningPlayer = (player_t)i;
        }
        // Otherwise, if there are units in this Player and the winning Player HAS been set
        else if ( units[i] > 0 )
        {
            // Set back to invalid and break out of the loop
            winningPlayer = player_t::NUM_INVALID;
            break;
        }
    }

    return winningPlayer;
}

// Check if any of the units can move
bool CTTRTSGame::UnitsCanMove() const
{
    for( const SOrderUnitPair& pair: m_OrderUnitPairs )
    {
        uvector2 pos = pair.unit.GetPos();

        // Assume if unit is adjacent to any valid tile, then it can move there
        if(        VerifyPosIsValidMovement(pos + vector2(1, 0) ) == 0
                || VerifyPosIsValidMovement(pos + vector2(0, 1)) == 0
                || VerifyPosIsValidMovement(pos + vector2(-1, 0)) == 0
                || VerifyPosIsValidMovement(pos + vector2(0, -1)) == 0 )
        {
            return true;
        }
    }

    return false;
}

// Check if the game is over
bool CTTRTSGame::GameOver() const
{
    return ( (GetWinningPlayer() != player_t::NUM_INVALID )     // We have a winning player
            || GetNumUnits() == 0
            || !UnitsCanMove() );                       // OR we have no units
}