#ifndef _GAME_H_
#define _GAME_H_

#include "unit.h"
#include "gametypes.h"
#include "orders.h"

#include <memory> // unique_ptr and shared_ptr

typedef std::vector< CUnit > CUnitVector;

// Type for order and unit pairs
struct OrderUnitPair
{
    const COrder& order;
    const CUnit& unit;
};

typedef std::vector< OrderUnitPair > OrderUnitPairVector;

class CTTRTSGame
{
public:

	CTTRTSGame( ucoord_t c, ucoord_t r )
	: dimentions( {c,r} )
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
	int AddUnit( CUnit&& unit );
		
	// Add a units, nonzero return value indicates error
	int AddUnits( CUnitVector&& units );

	// Get the number of units
	inline unsigned int GetNumUnits() const { return m_allUnits.size(); }

	// Get unit by index as above (not unit ID)
	inline const CUnit& GetUnitByIndex( unsigned int i ) const { return m_allUnits[i]; }
	
    // Get unit by unit ID
    const CUnit& GetUnitByID( unit_id_t id ) const;

	// Get the number of order
	inline unsigned int GetNumOrders() const { return m_orders.size(); }

	// Get orders by index as above 
	inline const COrder& GetOrdersByIndex( unsigned int i ) const { return m_orders[i]; }

	// Get dimentions
    inline const uvector2& GetDimentions() const { return dimentions; }
	
private:

	// Verify any order
    int VerifyOrder( player_id_t player, const COrder& order ) const;

	// Verify any order
    int VerifyUnit(  const CUnit& unit ) const;

	// Vector to store points to all units
	CUnitVector 		m_allUnits;

	// Orders to execute this turn
	COrderVector 		m_orders;

	// Dimensions of the game
	uvector2 dimentions;
};

#endif //_GAME_H_
