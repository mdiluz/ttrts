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
CBoard::CBoard( unsigned int c, unsigned int r, vunit_t&& b )
: cols ( c )
, rows ( r )
, total ( rows * cols )
, board ( std::move(b) )
{
	board.resize(total,square_empty);
}

// print get a slot on the board
unit_t CBoard::get( const unsigned int c, const unsigned int r ) const
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	return board[r*c];
}

// Get a square on the board
unit_t CBoard::set( const unsigned int c, const unsigned int r , const unit_t n )
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	unit_t old = board[r*c];
	board[r*c] = n;
	return old;
}