#include "game.h"


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

	m_orders.push_back(order);

	return 0;
}

// Simulate and progress to the next turn
// Returns non-zero if simulation failed
int CTTRTSGame::SimulateToNextTurn()
{
	int error;

    OrderUnitPairVector orderPairs;

    // Grab all movement orders
    for ( auto order : m_orders )
    {
        const OrderUnitPair pair = { order, GetUnitByID(order.unit) };
        orderPairs.push_back(pair);
    }

    // Attempt all movement orders
    for ( auto pair : orderPairs )
    {
        switch (  pair.order.order )
        {
        case order_c::F:
            {
                // Verify new unit position will be on the board
            }
            break;
        }
    }

    // Vector of units to kill
    std::vector< unit_id_t > toKill;

    // Attempt all actions
    for ( auto pair : orderPairs )
    {
        switch (  pair.order.order )
        {
        case order_c::A:
            {
                // Verify that there's a unit in front to attack
            }
            break;
        case order_c::L:
        case order_c::R:
            // Nothing needed here, these orders can always be carried out
            break;
        }
    }

    for ( auto id : toKill )
    {

    }

	// Clear all orders
	m_orders.resize(0);

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
	if( (pos.x < dimentions.x) && (pos.y < dimentions.y) )
		return 1;

	m_allUnits.push_back(std::move(unit));

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
	// Grab the unit ID
	const unit_id_t unitID = order.unit;

	// Attempt to find the unit
	bool unitFound = false;
    for ( const CUnit& unit : m_allUnits )
	{
        if ( unit.getID() == unitID )
		{
			unitFound = true;
			break;
		}
	}

	// for now, as long as the unit exists we can attempt the order
	return unitFound;
}


// Get unit by unit ID
const CUnit& CTTRTSGame::GetUnitByID( unit_id_t id ) const
{
    CUnitVector::const_iterator it;

    for ( const CUnit& unit : m_allUnits )
    {
        // Attempt the unit add
        if ( unit.getID()  )
            return unit;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}
