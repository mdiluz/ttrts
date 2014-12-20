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
int CTTRTSGame::IssueOrders( Team team, const std::string& _orders )
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
	return IssueOrders(team,orderVector);
}

// Issue orders by vector to the game
int CTTRTSGame::IssueOrders( Team team, const COrderVector& orders )
{
	// verify all the orders
    for ( auto order : orders )
	{
        // If any order returns non-zero, back out
        if ( IssueOrder(team,order) )
			return 1;
	}

	return 0;
}

// Issue a single order
int CTTRTSGame::IssueOrder( Team team, const COrder& order )
{
    // Verify the order
	if ( VerifyOrder(team,order) )
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
                        // Skip myself
                        if( pair.unit.getID() == pair2.unit.getID() ) continue;

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
                    pair.unit.setPos(newpos);
                }
            }
            break;
        default:
            break;
        }
    }

    // Turn all units that need turning
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        switch (  pair.order.command)
        {
            case command_c::L:
            {
                // Simply turn left
                pair.unit.turnLeft();
            }
                break;
            case command_c::R:
            {
                // Simply turn right
                pair.unit.turnRight();
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
        for ( OrderUnitPair& pair : m_OrderUnitPairs )
        {
            if ( pair.order.command == command_c::A )
            {
                uvector2 newpos = pair.unit.getInFront();
                // If move would be within the arena
                if ( ( newpos.x <= dimensions.x-1 ) && ( newpos.y <= dimensions.y-1 ) )
                {
                    pair.unit.setPos(newpos);

                    // Unit moved, so more charging needs to be done
                    charging = true;
                }
            }
        }

        std::vector< unit_id_t > toKill; // Vector to store which units to kill

        // Initially move all units to check for pass through
        for ( OrderUnitPair& pair1 : m_OrderUnitPairs )
        if ( pair1.order.command == command_c::A )
        for ( OrderUnitPair& pair2 : m_OrderUnitPairs )
        if ( pair1.unit.getID() != pair2.unit.getID() // Don't check the same units
                && pair2.order.command == command_c::A )
        {
            if( CheckForPassThrough(pair1.unit,pair2.unit) )
            {
                toKill.push_back(pair1.unit.getID());
                toKill.push_back(pair2.unit.getID());
            }
        }

        // Kill all units to kill
        KillAll(toKill);
        toKill.clear();

        // Check for all matching spots
        for ( OrderUnitPair& pair1 : m_OrderUnitPairs )
        for ( OrderUnitPair& pair2 : m_OrderUnitPairs )
        {
            if( pair1.unit.getID() == pair2.unit.getID() ) continue; // Don't check the same units

            if( pair1.unit.getPos() ==  pair2.unit.getPos() )
            {
                if( pair1.order.command == command_c::A )
                {
                    toKill.push_back(pair2.unit.getID());
                }

                if( pair2.order.command == command_c::A )
                {
                    toKill.push_back(pair1.unit.getID());
                }
            }
        }

        // Kill all units to kill
        KillAll(toKill);
        toKill.clear();
    }

    // Clear all orders
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
        pair.order = COrder();

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
            if( (*it).unit.getID() == id )
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
    uvector2 pos1 = one.getPos();
    uvector2 pos2 = two.getPos();
    dir_t dir1 = one.getDir();
    dir_t dir2 = two.getDir();

    if( pos1.x == pos2.x ) { // Same col
        if (pos1.y == (pos2.y + 1)) {
            if (dir1 == dir_t::N && dir2 == dir_t::S)
                return true;
        }
        else if (pos1.y == (pos2.y - 1)) {
            if (dir1 == dir_t::S && dir2 == dir_t::N)
                return true;
        }
    }
    else if( pos1.y == pos2.y ) { // Same row
        if( pos1.x == (pos2.x+1) ) {
            if( dir1 == dir_t::E && dir2 == dir_t::W )
                return true;
        }
        else if( pos1.x == (pos2.x-1) ) {
            if( dir1 == dir_t::E && dir2 == dir_t::W )
                return true;
        }
    }

    return false;
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
int CTTRTSGame::VerifyOrder( Team team, const COrder& order ) const
{
    int ret = 1;

	// Grab the unit ID
	const unit_id_t unitID = order.unit;

    // Attempt to find the unit
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
	{
        // Accept if we have the unit
        if ( pair.unit.getID() == unitID
                && pair.unit.getTeam() == team )
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
        if ( pair.unit.getID() == id  )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Get an order by unit ID
const COrder& CTTRTSGame::GetOrderByIDConst( unit_id_t id ) const
{
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if ( pair.unit.getID() == id  )
            return pair.order;
    }

    // Return an invalid unit
    static COrder invalid_order;
    return invalid_order;
}

// Get unit by unit ID
CUnit& CTTRTSGame::GetUnitByID( unit_id_t id )
{
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        // Attempt the unit add
        if ( pair.unit.getID() == id )
            return pair.unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Get a vector of the teams in the current game
std::vector<Team> CTTRTSGame::GetTeams() const
{
    std::vector<Team> teams;
    teams.reserve(GetNumUnits());

    // Grab all teams
    for ( const OrderUnitPair& pair : m_OrderUnitPairs )
    {
        teams.push_back(pair.unit.getTeam());
    }

    // Remove dupes
    std::sort( teams.begin(), teams.end() );
    teams.erase( std::unique( teams.begin(), teams.end() ), teams.end() );

    return teams;
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
    state += "END";

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