#ifndef _GAME_H_
#define _GAME_H_

#include "unit.h"
#include "gametypes.h"
#include "orders.h"

#include <memory> // unique_ptr and shared_ptr

typedef std::vector< std::shared_ptr<CUnit> > sharedUnitVector_t;

class CTTRTSGame
{
public:

	CTTRTSGame( ucoord_t c, ucoord_t r )
	: cols (c)
	, rows (r)
	{

	}

	// Default dtor
	~CTTRTSGame() = default;

	// Issue orders to the game, returns non-zero if orders are incorrect
	int IssueOrders( player_id_t player, const std::string& orders );
	int IssueOrders( player_id_t player, const COrderVector& orders );

	int IssueOrder( player_id_t player, const COrder& order );

	// Simulate and progress to the next turn
	// Returns non-zero if simulation failed
	int SimulateToNextTurn();

	// Add a unit, nonzero return value indicates error
	int AddUnit( std::shared_ptr<CUnit> unit );
		
	// Add a units, nonzero return value indicates error
	int AddUnits( sharedUnitVector_t units );

	// Get the number of units
	inline unsigned int GetNumUnits() const { return m_allUnits.size(); }

	// Get unit by index as above (not unit ID)
	inline const CUnit& GetUnitByIndex( unsigned int i ) const { return *m_allUnits[i]; }
	
	// Get the number of order
	inline unsigned int GetNumOrders() const { return m_orders.size(); }

	// Get orders by index as above 
	inline const COrder& GetOrdersByIndex( unsigned int i ) const { return m_orders[i]; }
	
private:

	// Simulate all movements
	int SimulateMovements();

	// Simulate all actions
	int SimulateActions();

	// Verify any order
	int VerifyOrder( player_id_t player, const COrder& order );

	// Verify any order
	int VerifyUnit(  const CUnit& unit );

	// Vector to store points to all units
	sharedUnitVector_t 	m_allUnits;

	// Orders to execute this turn
	COrderVector 		m_orders;

	// Dimensions of the game
	ucoord_t rows;
	ucoord_t cols;
};

#endif //_GAME_H_