#include "orders.h"

#include "mathtypes.h"

// Convert an order to a string
std::string GetStringFromOrder(const COrder& order )
{
	std::string ret;
	ret += std::to_string(order.unit);
	ret += ORDER_DELIMITER;
	ret += order.order;

	return ret;
}

// Convert a string to an order
COrder GetOrderFromString( const std::string& _order )
{
	std::string order = _order;
	COrder ret;

	int pos = order.find(ORDER_DELIMITER);
	if( pos != std::string::npos )
	{
		const std::string order_unit = order.substr(0, pos);

		ret.unit = atoi( order_unit.c_str() );

		// Erase everything up to and including the delimiter
		order.erase(0, pos + 1);

		// Next single char is the order
		ret.order = order[0];
	}

	return ret;
}

bool isMovementOrder( const COrder& order )
{
	const order_c c = order.order;
	for ( unsigned int i = 0; i < _countof(sk_movementOrders); i++ )
	{
		if ( c == sk_movementOrders[i] )
			return true;
	}
	return false;
}

bool isActionOrder( const COrder& order )
{
	const order_c c = order.order;
	for ( unsigned int i = 0; i < _countof(sk_actionOrders); i++ )
	{
		if ( c == sk_actionOrders[i] )
			return true;
	}
	return false;
}