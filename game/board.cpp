#include "board.h"

#include <iostream>     // std::cout

// ----------------------------------------------

// Default constructor for the board
CBoard::CBoard( unsigned int c, unsigned int r )
: cols ( c )
, rows ( r )
, total ( rows * cols )
{
	board = new square_t[total];
	fill(square_invalid);
}

CBoard::~CBoard()
{
	delete[] board;
}

// Clear the board
void CBoard::clear()
{
	fill(square_empty);
}

// Fill the board
void CBoard::fill(square_t v)
{
	std::fill(board,board+total,v);
}

// print get a slot on the board
square_t CBoard::get( unsigned int c, unsigned int r ) const
{
	if ( (r >= rows) || (c >= cols) )
		return square_invalid;

	return board[r*c];
}

// print a board
void CBoard::debug_print() const
{
	for ( unsigned int r = 0; r < rows; r++ )
	{
		for ( unsigned int c = 0; c < cols; c++ )
		{
			std::cout<<(char)board[r*c];
		}
		std::cout<<std::endl;
	}
}

// Test the board data class 
void tests::test_CBoard()
{
	CBoard board = CBoard(20,10);

	std::cout<<"Blank board"<<std::endl;
	board.clear();
	board.debug_print();

	std::cout<<"Filled board"<<std::endl;
	board.fill(48);
	board.debug_print();
}