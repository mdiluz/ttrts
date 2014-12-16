#ifndef _GAME_H_
#define _GAME_H_

#include "unit.h"
#include "gametypes.h"
#include "order.h"

#define GAME_HEADER_FORMATTER "===== %s =====\nSIZE:[%u,%u]\nTURN:%u"
#define GAME_HEADER_DELIMITER "~~~~\n"

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

    // Move assignment operator
    inline OrderUnitPair& operator=( OrderUnitPair&& rhs )
    {
        this->unit = std::move(rhs.unit);
        this->order = std::move(rhs.order);
        return *this;
    }

    CUnit   unit;       // The unit
    COrder  order;      // Order for this unit from this turn
};

// Typedef for a vector of these unit pairs
typedef std::vector< OrderUnitPair > OrderUnitPairVector;

// Full TTRTS Game class
// Stores information about the game
// Can convert from a string or to a string
class CTTRTSGame
{
public:

    // Get the game information as a string
    static CTTRTSGame CreateFromString( const std::string& input );

    // Constructors
	CTTRTSGame( ucoord_t c, ucoord_t r );
    CTTRTSGame(CTTRTSGame&& game);

    // move asignment operator
    CTTRTSGame& operator=(CTTRTSGame&& game);

    // Simulate and progress to the next turn
    // Returns non-zero if simulation failed
    int SimulateToNextTurn();

    // Check for a win, returns invalid for no win state reached
    // Note: this function will return invalid a draw was reached
    //       best practice would be to call with GetNumUnits() == 0
    Team CheckForWin() const;

    // Get the game information as a string
    std::string GetStateAsString() const;

	// Issue orders to the game, returns non-zero if orders are incorrect
	int IssueOrders( Team team, const std::string& orders );
	int IssueOrders( Team team, const COrderVector& orders );
	int IssueOrder( Team team, const COrder& order );

	// Add a units to the game, nonzero return value indicates error
	int AddUnit( CUnit&& unit );
	int AddUnits( CUnitVector&& units );

	// Get the number of units
    inline unsigned int GetNumUnits() const { return m_OrderUnitPairs.size(); }

	// Get unit and orderby index as above (not unit ID)
    inline const CUnit& GetUnitByIndex( unsigned int i ) const { return m_OrderUnitPairs[i].unit; }
    inline const COrder& GetOrdersByIndex( unsigned int i ) const { return m_OrderUnitPairs[i].order; }

    // Get a unit by it's ID
    const CUnit& GetUnitByIDConst( unit_id_t id ) const;

    // Get dimensions
    inline const uvector2 &GetDimensions() const { return dimensions; }

    // Set the game name
    // NOTE: Names with spaces not allowed
    inline std::string SetName( const std::string& in ) { return (name = in); }
    inline std::string GetName() const                  { return name; }

    // Set the turn of the game
    inline int SetTurn( int in ) { return (turn = in); }
	
private:

    // Verify any order or position - non-zero is error
    int VerifyOrder( Team team, const COrder& order ) const;
    int VerifyPos( uvector2 vec ) const;

    // Get a units new position after an order
    uvector2 GetNewPosition( const OrderUnitPair& pair ) const;

    // Get unit by unit ID
    CUnit& GetUnitByID( unit_id_t id );

    std::string         name;               // Game Name
    unsigned int        turn;               // Int to store the current turn
    uvector2            dimensions;         // Dimensions of the game
    OrderUnitPairVector m_OrderUnitPairs;   // Vector to store all units and orders
};


#endif //_GAME_H_