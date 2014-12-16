#include <string.h>
#include "orders.h"

#include "mathtypes.h"

// Convert an order to a string
std::string GetStringFromOrder(const COrder& order )
{
	static char buff[128];
	memset(buff,0,sizeof(buff));

	snprintf(buff,128, ORDER_FORMATTER,
			order.command,
			order.unit);

	return buff;
}

// Convert a string to an order
COrder GetOrderFromString( const std::string& order )
{
	COrder ret;

	char corder;
	unsigned int unit;

	sscanf(order.c_str(), ORDER_FORMATTER,
			&corder,
			&unit);

	ret.command = (command_c)corder;
	ret.unit = unit;

	return ret;
}
