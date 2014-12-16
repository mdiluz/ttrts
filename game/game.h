#ifndef _GAME_H_
#define _GAME_H_

#include "unit.h"
#include "gametypes.h"
#include "order.h"

// Type for order and unit pairs
struct OrderUnitPair
{
    // Straight up move constructor
    OrderUnitPair( OrderUnitPair&& other )
        : unit ( std::move(other.unit) )
        , order ( other.order )
    {}

    // Multi parameter constructor
    OrderUnitPair( CUnit&& u, COrder o )
        : unit ( std::move(u) )
        , order ( o )
    {}

    // Move asignment operator
    inline OrderUnitPair& operator=( OrderUnitPair&& rhs ) { this->unit = std::move(rhs.unit);this->order = rhs.order;rhs.order = COrder(); return *this; }

    CUnit unit;     // The unit
    COrder order;   // Order for this unit from this turn
};

// Typedef for a vector of these unit pairs
typedef std::vector< OrderUnitPair > OrderUnitPairVector;

class CTTRTSGame
{
public:

	CTTRTSGame( ucoord_t c, ucoord_t r )
	: dimensions( c,r )
    , turn (0)
	{

	}

	// Default dtor
	~CTTRTSGame() = default;

	// Issue orders to the game, returns non-zero if orders are incorrect
	int IssueOrders( player_id_t player, const std::string& orders );
	int IssueOrders( player_id_t player, const COrderVector& orders );

    // Issue a single order, returns non-zero for rejection
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

	// Get dimensions
    inline const uvector2 &GetDimensions() const { return dimensions; }

    // Check for a win, returns invalid for no win state reached
    // Note: this function will return invalid a draw was reached
    //       best practice would be to call with GetNumUnits() == 0
    Team CheckForWin() const;

    // Get the game information as a string
    std::string GetStateAsString() const;
	
private:

    // Verify any order - non-zero is error
    int VerifyOrder( player_id_t player, const COrder& order ) const;

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
	uvector2 dimensions;

    // Int to store the current turn
    unsigned int turn;
};

#define GAME_HEADER_FORMATTER "===== GAME TURN =====\nSIZE:[%u,%u]\nTURN:%u"

#endif //_GAME_H_
