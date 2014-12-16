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
	for ( COrderVector::const_iterator it = orders.begin(); it != orders.end(); it++ )
	{
		if ( IssueOrder(player,*it) )
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

	// Simulate all movements first
	error = SimulateMovements();

	// Simulate all the actions
	error = SimulateActions();

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
	for ( CUnitVector::const_iterator it = m_allUnits.begin(); it != m_allUnits.end(); it++ )
	{
		if ( (*it).getID() == unitID )
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

    for ( it = m_allUnits.begin(); it != m_allUnits.end(); it++ )
    {
        // Attempt the unit add
        if ( (*it).getID()  )
            return *it;
    }

    // Return an invalid unit
    static CUnit invalid_unit;
    return invalid_unit;
}

// Verify an order unit pair
int CTTRTSGame::VerifyOrderUnitPair( const OrderUnitPair& pair ) const
{
    switch ( pair.order.order )
    {
    case order_c::F:
    {
        // Verify new unit position will be on the board
    }
        break;
    case order_c::L:
    case order_c::R:
    case order_c::A:
        // Nothing needed here, orders can always be carried out
        break;
    }

    return 0;
}

// Simulate all movements
int CTTRTSGame::SimulateMovements()
{
    OrderUnitPairVector movements;

    // Grab all movement orders
	for ( COrderVector::const_iterator it = m_orders.begin(); it != m_orders.end(); it++ )
	{
		if( isMovementOrder(*it) )
        {
            const OrderUnitPair pair = { *it, GetUnitByID((*it).unit) };
            movements.push_back(pair);
        }
	}

    // Remove all orders with straight up impossible movements
    for ( OrderUnitPairVector::const_iterator it = movements.begin(); it != movements.end(); it++ )
    {
        if( VerifyOrderUnitPair(*it) )
        {

        }
    }

	// Calculate movements

	return 0;
}

// Simulate all actions
int CTTRTSGame::SimulateActions()
{
	// Grab all action orders
	COrderVector actions;
	for ( COrderVector::const_iterator it = m_orders.begin(); it != m_orders.end(); it++ )
	{
		if( isActionOrder(*it) )
			actions.push_back(*it);
	}

	// Calculate actions

	return 0;
}
