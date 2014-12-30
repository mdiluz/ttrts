#ifndef _ORDERS_H_
#define _ORDERS_H_

#include <vector>
#include <string>

#include "gametypes.h"

#define ORDER_FORMATTER "ORDER:%c id:%u"

// Type for all orders ( as a char )
enum class command_c : char
{
    F = 'F', // Move forward one square
    L = 'L', // Turn left
    R = 'R', // Turn right
    A = 'A', // Attack forwards until a unit or edge of the arena is hit
    NUM_INVALID
};

// Container for an order
struct SOrder
{
    // Base constructor makes invalid order
    SOrder()
        : unit ( unit_id_invalid )
        , command( command_c::NUM_INVALID )
    {}

	// Unit order is for
	unit_id_t unit;

	// Order command issued
    command_c command;

	// Basic operators
	inline bool operator==( const SOrder & rhs ) const;
	inline bool operator!=( const SOrder & rhs ) const { return !(*this==rhs); }
};

// Simple == operator
inline bool SOrder::operator== ( const SOrder & rhs ) const
{
	return ( unit == rhs.unit ) && ( command == rhs.command);
}

// Typedef a vector of orders
typedef std::vector<SOrder> COrderVector;

#endif //_ORDERS_H_
