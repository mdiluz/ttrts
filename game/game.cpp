#include "game.h"
// ----------------------------------------------

// Default constructor for the board
CBoardData::CBoardData( unsigned int c, unsigned int r )
: cols ( c )
, rows ( r )
, total ( rows * cols )
{
	board = new square_t[total];
	fill(square_invalid);
}

CBoardData::~CBoardData()
{
	delete[] board;
}

// Clear the board
void CBoardData::clear()
{
	fill(square_empty);
}

// Fill the board
void CBoardData::fill(square_t v)
{
	std::fill(board,board+total,v);
}

// print get a slot on the board
CBoardData::square_t CBoardData::get( unsigned int c, unsigned int r ) const
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	return board[r*c];
}

// print a board
void CBoardData::print() const
{
	for ( unsigned int r = 0; r < rows; r++ )
	{
		for ( unsigned int c = 0; c < cols; c++ )
		{
			std::cout<<board[r*c];
		}
		std::cout<<std::endl;
	}
}

// Test the board data class 
void tests::test_CBoardData()
{
	CBoardData board = CBoardData(20,10);

	std::cout<<"Blank board"<<std::endl;
	board.clear();
	board.print();

	std::cout<<"Filled board"<<std::endl;
	board.fill(48);
	board.print();
}