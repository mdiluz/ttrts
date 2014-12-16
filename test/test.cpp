#include "board.h"

#include <iostream>     // std::cout

#include "unitv.h"

// Namespace for testing functions
namespace tests
{
	// print a board
	void debug_print( CBoard& b )
	{
		for ( unsigned int r = 0; r < b.rows; r++ )
		{
			for ( unsigned int c = 0; c < b.cols; c++ )
			{
				std::cout<<(char)(b.get(c,r));
			}
			std::cout<<std::endl;
		}
	}

	// Test the board data class 
	void test_CBoard()
	{
		CBoard board = CBoard(10,5);

		std::cout<<"Blank board"<<std::endl;
		board.clear();
		debug_print(board);

		std::cout<<"Filled board"<<std::endl;
		board.fill(48);
		debug_print(board);
	}

};



// Main program entry point
int main()
{
	tests::test_CBoard();

	{
		CUnitV myV;
		std::cout<<myV.getVisual()<<std::endl;
	}

	{
		std::unique_ptr<CUnit> myV = CUnit::getUnitFromVis('v');
		std::cout<<myV->getVisual()<<std::endl;
	}
};