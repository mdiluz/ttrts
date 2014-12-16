#include "orders.h"

#include "mathtypes.h"

// Convert an order to a string
std::string GetStringFromOrder(const COrder& order )
{
	std::string ret;
	ret += "O:";
	ret += (char)order.order;
	ret += " U:";
	ret += std::to_string(order.unit);

	return ret;
}

// Convert a string to an order
COrder GetOrderFromString( const std::string& _order )
{
	std::string order = _order;
	COrder ret;

	if( order.substr(0, 2) != "O:" )
		return COrder();
	order.erase(0, 2); // Erase the O: prefix

	ret.order = (order_c)order[0]; // Grab single char oder
	order.erase(0, 2); // Erase the order and a space

	if( order.substr(0, 2) != "U:" )
		return COrder();
	order.erase(0, 2); // Erase the U: prefix

	ret.unit = (unit_id_t)atoi( order.c_str() ); // Grab the unit ID

	return ret;
}
