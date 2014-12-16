#ifndef _BOARD_H_
#define _BOARD_H_

#include "basetypes.h"
#include "unit.h"

#include <limits>       // std::numeric_limits
#include <vector>   	// std::vector

typedef std::vector< unitType_c > vunitType_c;

// Invalid value for the board square
constexpr unitType_c square_invalid = std::numeric_limits<unitType_c>::max();
constexpr unitType_c square_empty = ' ';

// Class to store simple data about a board
class CBoard
{
public:

	const unsigned int cols; 		// Number of columns
	const unsigned int rows; 		// Number of rows
	const unsigned int total;		// Total number of pieces

	// constructor
	CBoard( unsigned int c, unsigned int r );

	// constructor
	CBoard( unsigned int c, unsigned int r, vunitType_c&& b );

	// Default destructor
	~CBoard() = default;

	// clear the board
	inline void clear() { fill(square_empty); }

	// fill the board
	inline void fill(unitType_c v) { std::fill(board.begin(),board.end(),v); };

	// Get a square on the board
	unitType_c get( const unsigned int c, const unsigned int r ) const;

	// Get the full board
	inline const vunitType_c& get() const { return board; };

	// Get a square on the board
	unitType_c set( const unsigned int c, const unsigned int r , const unitType_c n );

private:

	vunitType_c board; 	// Board data storage
}; 

#endif //_BOARD_H_