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
    // Straight up move constructor
    OrderUnitPair( OrderUnitPair&& other )
        : unit ( std::move(other.unit) )
        , order ( other.order )
    {

    }

    // Multi parameter constructor
    OrderUnitPair( CUnit&& u, COrder o )
        : unit ( std::move(u) )
        , order ( o )
    {

    }

    // Move asignment operator
    inline OrderUnitPair& operator=( OrderUnitPair&& rhs ) { this->unit = std::move(rhs.unit);this->order = rhs.order;rhs.order = COrder(); return *this; }

    CUnit unit;
    COrder order;
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
    inline unsigned int GetNumUnits() const { return m_OrderUnitPairs.size(); }

	// Get unit by index as above (not unit ID)
    inline const CUnit& GetUnitByIndex( unsigned int i ) const { return m_OrderUnitPairs[i].unit; }
	
    // Get unit by unit ID
    const CUnit& GetUnitByIDConst( unit_id_t id ) const;

	// Get orders by index as above 
    inline const COrder& GetOrdersByIndex( unsigned int i ) const { return m_OrderUnitPairs[i].order; }

	// Get dimentions
    inline const uvector2& GetDimentions() const { return dimentions; }
	
private:

    // Verify any order - non-zero is error
    int VerifyOrder( player_id_t player, const COrder& order ) const;

    // Verify any order - non-zero is error
    int VerifyUnit(  const CUnit& unit ) const;

    // Verify Position - non-zero is error
    int VerifyPos( uvector2 vec ) const;

    // Get a units new position after an order
    uvector2 GetNewPosition( const OrderUnitPair& pair ) const;

    // Get unit by unit ID
    CUnit& GetUnitByID( unit_id_t id );

	// Vector to store points to all units
    OrderUnitPairVector 		m_OrderUnitPairs;

    // List of dead units
    CUnitVector                 m_deadUnits;

	// Dimensions of the game
	uvector2 dimentions;
};

#endif //_GAME_H_
