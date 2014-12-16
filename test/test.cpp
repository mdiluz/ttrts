#include <iostream>     // std::cout

#include "unitv.h"
#include "board.h"
#include "orders.h"


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
		std::cout<<"Basic V unit construction"<<std::endl;
		CUnitV myV;
		std::cout<<myV.getVisual()<<std::endl;
	}

	{
		std::cout<<"CUnit factory V unit construction"<<std::endl;
		std::unique_ptr<CUnit> myV = CUnit::getUnitFromVis('v');
		if( myV->getVisual() != 'v' )
			std::cout<<"ERROR, failed to properly create V unit"<<std::endl;
	}

	{
		std::cout<<"COrder construction and conversion"<<std::endl;
		COrder order;
		order.order = 'F';
		order.unit = 10;
		std::string order_string = GetStringFromOrder(order);
		std::cout<<order_string<<std::endl;
		COrder order2 = GetOrderFromString(order_string);

		if ( order2 != order )
			std::cout<<"ERROR, failed order string conversion test"<<std::endl;
	}
};