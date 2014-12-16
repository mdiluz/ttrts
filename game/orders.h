#ifndef _ORDERS_H_
#define _ORDERS_H_

#include <vector>
#include <string>

#include "gametypes.h"

#define ORDER_DELIMITER ' '

// Type for all orders ( as a char )
typedef char order_c;

// Movement orders
static const order_c sk_movementOrders[] = 
{
	'F',	// Forward
};

// Action orders
static const order_c sk_actionOrders[] = 
{
	'L',	// Left
	'R',	// Right
	'A',	// Attack
};

// Container for an order
struct COrder
{
	// Unit order is for
	unit_id_t unit;

	// Order command issued
	order_c order;

	// Basic operators
	inline bool operator==( const COrder& rhs ) const;
	inline bool operator!=( const COrder& rhs ) const { return !(*this==rhs); }
};

// Simple == operator
inline bool COrder::operator== ( const COrder& rhs ) const
{
	return ( unit == rhs.unit ) && ( order == rhs.order );
}

// Typedef a vector of orders
typedef std::vector<COrder> COrderVector;

// Order strings stored as simply "[unit id] [order char]"
// string <--> order conversion functions
std::string GetStringFromOrder(const COrder& order );
COrder GetOrderFromString( const std::string& order );

bool isMovementOrder( const COrder& order );
bool isActionOrder( const COrder& order );

#endif //_ORDERS_H_