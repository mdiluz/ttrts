#ifndef _ORDERS_H_
#define _ORDERS_H_

#include <vector>
#include <string>

#include "gametypes.h"

#define ORDER_FORMATTER "ORDER:%c id:%u"

// Type for all orders ( as a char )
enum class command_c : char
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
        , command( command_c::NUM_INVALID )
    {}

	// Unit order is for
	unit_id_t unit;

	// Order command issued
    command_c command;

	// Basic operators
	inline bool operator==( const COrder& rhs ) const;
	inline bool operator!=( const COrder& rhs ) const { return !(*this==rhs); }
};

// Simple == operator
inline bool COrder::operator== ( const COrder& rhs ) const
{
	return ( unit == rhs.unit ) && ( command == rhs.command);
}

// Typedef a vector of orders
typedef std::vector<COrder> COrderVector;

// string <--> order conversion functions
std::string GetStringFromOrder(const COrder& order );
COrder GetOrderFromString( const std::string& order );

#endif //_ORDERS_H_
