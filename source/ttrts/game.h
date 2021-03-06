#ifndef _GAME_H_
#define _GAME_H_

#include "unit.h"
#include "gametypes.h"
#include "order.h"
#include "orderunitpair.h"

// Full TTRTS Game class
// Stores information about the game
// Can convert from a string or to a string
class CTTRTSGame
{
public:

    // Constructors
	CTTRTSGame( ucoord_t c, ucoord_t r );
    CTTRTSGame(CTTRTSGame&& game);

    // move asignment operator
    CTTRTSGame&             operator=(CTTRTSGame&& game);

    // Simulate and progress to the next turn
    // Returns non-zero if simulation failed
    int                     SimulateToNextTurn();

    // Check for winning player, returns invalid for no win state reached
    // Note: this function will return invalid if a draw was reached
    //       do not rely on this to test for end state
    player_t                GetWinningPlayer() const;

    // Check if the game is over
    bool                    GameOver() const;

    // Check if any of the units can move
    bool                    UnitsCanMove() const;

	// Issue orders to the game, returns non-zero if orders are incorrect
	int                     IssueOrders( player_t player, const std::string& orders );
	int                     IssueOrders( player_t player, const COrderVector& orders );
	int                     IssueOrder( player_t player, const SOrder & order );

	// Add a units to the game, nonzero return value indicates error
	int                     AddUnit( CUnit&& unit );
	int                     AddUnits( CUnitVector&& units );

	// Get the number of units
    inline unsigned int     GetNumUnits() const { return m_OrderUnitPairs.size(); }

	// Get unit and orderby index as above (not unit ID)
    inline const CUnit&     GetUnitByIndex( unsigned int i ) const      { return m_OrderUnitPairs[i].unit; }
    inline const SOrder &   GetOrdersByIndex( unsigned int i ) const   { return m_OrderUnitPairs[i].order; }

    // Get a unit by it's ID
    const CUnit&            GetUnitByIDConst( unit_id_t id ) const;
    const SOrder &          GetOrderByIDConst( unit_id_t id ) const;

    inline const OrderUnitPairVector& GetOrderUnitPairs() const        { return m_OrderUnitPairs; }

    // Get dimensions
    inline const uvector2&  GetDimensions() const { return dimensions; }

    // Set the game name
    // NOTE: Names with spaces not allowed
    inline std::string      SetName( const std::string& in ) { return (name = in); }
    inline std::string      GetName() const                  { return name; }

    // Set the turn of the game
    inline int              SetTurn( int in ) { return (turn = in); }
    inline int              GetTurn() const   { return turn; }

    // Get a vector of the players in the current game
    std::vector<player_t>   GetPlayers() const;

    // Get the vector of wall positions
    inline std::vector<uvector2>    GetWalls() const { return m_walls; }

    // Add an invalid position
    inline void                     AddWall(uvector2 vec) { m_walls.push_back(vec); }

private:

    // Check for a pass through
    static bool             CheckForPassThrough( const CUnit& one, const CUnit& two );

    // Verify any order or position - non-zero is error
    int                     VerifyOrder( player_t player, const SOrder & order ) const;
    int                     VerifyPosIsValidMovement(uvector2 vec) const;

    // Get a units new position after an order
    uvector2                GetNewPosition( const SOrderUnitPair & pair ) const;

    // Kill all units in list
    void                    KillAll( std::vector< unit_id_t >& vec );

    // Get unit by unit ID
    CUnit&                  GetUnitByID( unit_id_t id );

    std::string             name;               // Game Name
    unsigned int            turn;               // Int to store the current turn
    uvector2                dimensions;         // Dimensions of the game
    OrderUnitPairVector     m_OrderUnitPairs;   // Vector to store all units and orders
    std::vector<uvector2>   m_walls;            // Vector of wall positions
};


#endif //_GAME_H_
