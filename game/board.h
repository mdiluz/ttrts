#ifndef _BOARD_H_
#define _BOARD_H_

#include "basetypes.h"
#include <limits>       // std::numeric_limits

// Class to store simple data about a board
class CBoard
{
public:

	// Invalid value for the board square
	static const square_t square_invalid = std::numeric_limits<square_t>::max();
	static const square_t square_empty = 32; // 32 is ascii empty

	// Default constructor
	CBoard( unsigned int c, unsigned int r );
	~CBoard();

	// Print the board
	void debug_print() const;

	// clear the board
	void clear();

	// fill the board
	void fill(square_t v);

	// Get a square on the board
	square_t get( unsigned int c, unsigned int r ) const;

private:

	const unsigned int cols; 		// Number of columns
	const unsigned int rows; 		// Number of rows
	const unsigned int total;		// Total number of pieces

	square_t* board; 	// Board data storage
}; 

// Namespace for testing functions
namespace tests
{
	void test_CBoard();
};

#endif //_BOARD_H_