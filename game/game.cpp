#include "game.h"

#include <algorithm>

// Interpret a string of orders
int CTTRTSGame::IssueOrders( player_id_t player, const std::string& _orders )
{
	COrderVector orderVector;
	std::string orders = _orders;

	size_t pos;
	while ( (pos = orders.find("\n")) != std::string::npos ) 
	{
		const std::string sorder = orders.substr(0, pos);
		orders.erase(0,pos+1);

		COrder order = GetOrderFromString( sorder );

		orderVector.push_back(order);
	}

	return IssueOrders(player,orderVector);
}

// Issue orders by vector to the game
int CTTRTSGame::IssueOrders( player_id_t player, const COrderVector& orders )
{
	// verify all the orders
    for ( auto order : orders )
	{
        if ( IssueOrder(player,order) )
			return 1;
	}

	return 0;
}

// Issue a single order
int CTTRTSGame::IssueOrder( player_id_t player, const COrder& order )
{
	if ( VerifyOrder(player,order) )
			return 1;

    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        if ( pair.unit.getID() == order.unit )
        {
            pair.order = order;
            return 0;
        }
    }

    return 1;
}

// Verify a position
int CTTRTSGame::VerifyPos(uvector2 vec) const
{
    if ( ( vec.x >= dimentions.x )
        || ( vec.y >= dimentions.y ) )
    {
        return 1;
    }

    return 0;
}


// Get a units new position
uvector2 CTTRTSGame::GetNewPosition( const OrderUnitPair& pair ) const
{
    switch ( pair.order.order )
    {
    case order_c::F:
        return pair.unit.getInFront();
        break;
    default:
        break;
    }

    return { ucoord_invalid,ucoord_invalid };
}

// Simulate and progress to the next turn
// Returns non-zero if simulation failed
int CTTRTSGame::SimulateToNextTurn()
{
    int error = 0;

    // Attempt all movement orders
    for ( OrderUnitPair& pair : m_OrderUnitPairs )
    {
        switch (  pair.order.order )
        {
        case order_c::F:
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
        switch (  pair.order.order )
        {
        case order_c::A:
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
        case order_c::L:
            {
                pair.unit.turnLeft();
                pair.order = COrder();
            }
            break;
        case order_c::R:
            {
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
                // Add the dead unit to our dead unit list
                m_deadUnits.push_back(std::move((*it).unit));

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

	return error;
}

// Add a unit, nonzero return value indicates error
int CTTRTSGame::AddUnit( CUnit&& unit )
{	
	// Verify the unit
	const int val = unit.valid();
	if( val )
		return val;

	// Verify if the unit can be placed on the current board
	const uvector2 pos = unit.getPos();
    if( (pos.x >= dimentions.x) || (pos.y >= dimentions.y) )
		return 1;

    for ( const OrderUnitPair& pair: m_OrderUnitPairs )
    {
        if( pair.unit.getPos() == unit.getPos() )
            return 2;
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
    CUnitVector::iterator it;

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
    CUnitVector::iterator it;

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
