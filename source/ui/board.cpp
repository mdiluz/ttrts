#include "board.h"

// ----------------------------------------------

// Default constructor for the board
CBoard::CBoard( unsigned int c, unsigned int r )
: cols ( c )
, rows ( r )
, total ( rows * cols )
{
	board.resize(total,square_empty);
}


// constructor
CBoard::CBoard( unsigned int c, unsigned int r, vunitVis_c&& b )
: cols ( c )
, rows ( r )
, total ( rows * cols )
, board ( std::move(b) )
{
	board.resize(total,square_empty);
}

// print get a slot on the board
unitVis_c CBoard::get( const unsigned int c, const unsigned int r ) const
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	return board[r*c];
}

// Get a square on the board
unitVis_c CBoard::set( const unsigned int c, const unsigned int r , const unitVis_c n )
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	unitVis_c old = board[r*c];
	board[r*c] = n;
	return old;
}