#ifndef _ORDERS_H_
#define _ORDERS_H_

#include <vector>
#include <string>

#include "gametypes.h"

#define ORDER_DELIMITER ' '

// Type for all orders ( as a char )
enum class order_c : char
{
    F = 'F',
    L = 'L',
    R = 'R',
    A = 'A',
    INVALID
};

// Movement orders
static const order_c sk_movementOrders[] = 
{
    order_c::F,	// Forward
};

// Action orders
static const order_c sk_actionOrders[] = 
{
    order_c::L,	// Left
    order_c::R,	// Right
    order_c::A,	// Attack
};

// Container for an order
struct COrder
{
    COrder()
        : unit ( unit_id_invalid )
        , order ( order_c::INVALID )
    {

    }

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
