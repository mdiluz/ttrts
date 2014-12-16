#ifndef _ORDERS_H_
#define _ORDERS_H_

#include <vector>
#include <string>

#include "gametypes.h"

// Type for all orders ( as a char )
enum class order_c : char
{
    F = 'F',
    L = 'L',
    R = 'R',
    A = 'A',
    NUM_INVALID
};

// Container for an order
struct COrder
{
    // Base constructor makes invalid order
    COrder()
        : unit ( unit_id_invalid )
        , order ( order_c::NUM_INVALID )
    {}

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

// Order strings stored as simply "O:[order char] U:[unit id]"
// string <--> order conversion functions
std::string GetStringFromOrder(const COrder& order );
COrder GetOrderFromString( const std::string& order );

#endif //_ORDERS_H_
