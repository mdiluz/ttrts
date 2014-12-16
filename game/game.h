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

	CTTRTSGame();
	~CTTRTSGame() = default;

	// Initialise the game with default configuration
	void Initialise();

	// Issue orders to the game
	bool IssueOrders( std::string orders );
	bool IssueOrders( COrderVector orders );

	// Simulate and progress to the next turn
	bool NextTurn();

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
	bool SimulateMovements();

	// Simulate all actions
	bool SimulateActions();

	// Vector to store points to all units
	sharedUnitVector_t m_allUnits;

	// Orders to execute this turn
	COrderVector m_orders;
};

#endif //_GAME_H_