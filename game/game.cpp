#include "game.h"

#include <algorithm>
#include <string.h>

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
int CTTRTSGame::IssueOrders( player_id_t player, const std::string& _orders )
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
		COrder order = GetOrderFromString( sorder );
		orderVector.push_back(order);
	}

    // Call our add order by vector method
	return IssueOrders(player,orderVector);
}

// Issue orders by vector to the game
int CTTRTSGame::IssueOrders( player_id_t player, const COrderVector& orders )
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
int CTTRTSGame::IssueOrder( player_id_t player, const COrder& order )
{
    // Verify the order
	if ( VerifyOrder(player,order) )
			return 1;

    // Get the right unit for the order
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        if ( pair.unit.getID() == order.unit )
        {
            pair.order = order;
            return 0;
        }
    }

    // Unit was not found, return 2
    return 2;
}

// Verify a position
int CTTRTSGame::VerifyPos(uvector2 vec) const
{
    // Simply check if within the bounds of our dimensions for now
    if ( ( vec.x >= dimensions.x )
        || ( vec.y >= dimensions.y ) )
    {
        return 1;
    }

    return 0;
}


// Get a units new position
uvector2 CTTRTSGame::GetNewPosition( const OrderUnitPair& pair ) const
{

    // Grab the order
    switch ( pair.order.command)
    {
    // For forward orders, grab in front
    case command_c::F:
        return pair.unit.getInFront();
        break;
    // For all other orders, just grab the old position
    default:
        return pair.unit.getPos();
    }
}

// Simulate and progress to the next turn
// Returns non-zero if simulation failed
int CTTRTSGame::SimulateToNextTurn()
{
    int error = 0;

    // Attempt all movement orders
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        switch (  pair.order.command)
        {
        case command_c::F:
            {
                // Verify new unit position will be on the board
                uvector2 newpos = GetNewPosition(pair);

                // Verify the position is even available
                bool possible = ( VerifyPos(newpos) == 0 );

                if ( possible )
                {
                    // If any unit is in this spot, or moving unit moving to said spot, reject this
                    for ( const OrderUnitPair& pair2 : m_OrderUnitPairs )
                    {
                        if( GetNewPosition(pair2) != newpos )
                        {
                            possible = false;
                            break;
                        }
                    }
                }

                // If the movement is still possible
                if ( possible )
                {
                    pair.unit.setPos(newpos);
                    pair.order = COrder();
                }
            }
            break;
        default:
            break;
        }
    }

    // Vector of units to kill
    std::vector< unit_id_t > toKill;

    // Attempt all actions
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        switch (  pair.order.command)
        {
        case command_c::A:
            {
                // Verify that there's a unit in front to attack
                uvector2 infront = pair.unit.getInFront();

                // Check if there's any unit in front
                // FRIENDLY FIRE IS ENABLED
                for ( const OrderUnitPair& pair2 : m_OrderUnitPairs )
                {
                    // if the unit is infront of our unit, then add it to the kill list
                    if( pair2.unit.getPos() == infront )
                    {
                        toKill.push_back(pair2.unit.getID());
                        pair.order = COrder();
                        break;
                    }
                }
            }
            break;
        case command_c::L:
            {
                // Simply turn left
                pair.unit.turnLeft();
                pair.order = COrder();
            }
            break;
        case command_c::R:
            {
                // Simply turn right
                pair.unit.turnRight();
                pair.order = COrder();
            }
            break;
        default:
            break;
        }
    }

    // Sort and erase all duplicates
    std::sort( toKill.begin(), toKill.end() );
    toKill.erase( std::unique( toKill.begin(), toKill.end() ), toKill.end() );

    // Iterate through all kill orders
    for ( auto id : toKill )
    {
        // Kill the units
        for ( OrderUnitPairVector::iterator it = m_OrderUnitPairs.begin();
              it != m_OrderUnitPairs.end();
              it++ )
        {
            if( (*it).unit.getID() == id )
            {
                // Remove the unit from our alive unit pairs
                m_OrderUnitPairs.erase(it);
                break;
            }
        }

    }

	// Clear all orders
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        pair.order = COrder();
    }

    // Increment the current turn
    turn++;

	return error;
}

// Add a unit, nonzero return value indicates error
int CTTRTSGame::AddUnit( CUnit&& unit )
{	
	// Verify the unit
	if( !unit.valid() )
		return 1;

	// Verify if the unit can be placed on the current board
	const uvector2 pos = unit.getPos();
    if( (pos.x >= dimensions.x) || (pos.y >= dimensions.y) )
		return 2;

    // If any unit's position matches, reject this
    for ( const OrderUnitPair& pair: m_OrderUnitPairs )
    {
        if( pair.unit.getPos() == unit.getPos() )
            return 3;
    }

    // Add the unit with a blank order
    m_OrderUnitPairs.push_back( OrderUnitPair(std::move(unit), COrder()) );


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
int CTTRTSGame::VerifyOrder( player_id_t player, const COrder& order ) const
{
    int ret = 1;

	// Grab the unit ID
	const unit_id_t unitID = order.unit;

    // Attempt to find the unit
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
	{
        // Accept if we have the unit
        if ( pair.unit.getID() == unitID )
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
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if ( pair.unit.getID()  )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Get unit by unit ID
CUnit& CTTRTSGame::GetUnitByID( unit_id_t id )
{
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if ( pair.unit.getID()  )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Check if we have a win state
Team CTTRTSGame::CheckForWin() const
{
    // Array of units for each Team
    unsigned int units[(int) Team::NUM_INVALID];
    memset(units,0,sizeof(units));

    // Count up all the units for each Team
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
    {
        const int team = (int)pair.unit.getTeam();
        units[team] += 1;
    }

    // Default winning Team to invalid (no win)
    Team winningTeam = Team::NUM_INVALID;

    // For each of the teams
    for ( unsigned int i = 0; i < _countof(units); i++ )
    {
        // if there are still units in this Team, and the winning Team hasn't been set
        if( units[i] > 0 && winningTeam == Team::NUM_INVALID )
        {
            winningTeam = (Team)i;
        }
        // Otherwise, if there are units in this Team and the winning Team HAS been set
        else if ( units[i] > 0 )
        {
            // Set back to invalid and break out of the loop
            winningTeam = Team::NUM_INVALID;
            break;
        }
    }

    return winningTeam;
}

// Get the game information as a string
std::string CTTRTSGame::GetStateAsString() const
{
    // Print out the header
    char header[64];
    snprintf(header, 512, GAME_HEADER_FORMATTER , name.c_str(), dimensions.x, dimensions.y, turn );

    // Gather unit information
    std::string units;
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
    {
        units += CUnit::GetStringFromUnit(pair.unit);
        units += '\n';
    }

    // Append the header and units
    std::string state(header);
    state += '\n';
    state += GAME_HEADER_DELIMITER;
    state += units;

    return state;
}

// Get the game information as a string
CTTRTSGame CTTRTSGame::CreateFromString( const std::string& input )
{
    size_t headerEnd = input.find(GAME_HEADER_DELIMITER);

    std::string header = input.substr(0, headerEnd);
    std::string units = input.substr(headerEnd + strlen(GAME_HEADER_DELIMITER));

    // Grab information from the header
    char buf[64];
    unsigned int turn;
    unsigned int sizex;
    unsigned int sizey;
    sscanf(header.c_str(), GAME_HEADER_FORMATTER, buf, &sizex, &sizey, &turn );

    CTTRTSGame game(sizex,sizey);
    game.SetName(buf);
    game.SetTurn(turn);

    // For each line, construct a unit
    size_t pos;
    while ( ( pos = units.find('\n') ) != std::string::npos )
    {
        std::string unit_string = units.substr(0,pos);
        units.erase(0,pos+1);
        game.AddUnit(CUnit::GetUnitFromString(unit_string));
    }

    return game;
}