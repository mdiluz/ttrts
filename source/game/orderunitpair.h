#ifndef _TTRTS_ORDERUNITPAIR_H_
#define _TTRTS_ORDERUNITPAIR_H_

#include "order.h"
#include "unit.h"

#include <vector>

// Type for order and unit pairs
struct SOrderUnitPair
{
    // Straight up move constructor
    SOrderUnitPair( SOrderUnitPair && other )
            : unit ( std::move(other.unit) )
            , order ( other.order )
    {}

    // Multi parameter constructor
    SOrderUnitPair( CUnit&& u, COrder o )
            : unit ( std::move(u) )
            , order ( o )
    {}

    // Move assignment operator
    inline SOrderUnitPair & operator=( SOrderUnitPair && rhs )
    {
        this->unit = std::move(rhs.unit);
        this->order = std::move(rhs.order);
        return *this;
    }

    CUnit   unit;       // The unit
    COrder  order;      // Order for this unit from this turn
};

// Typedef for a vector of these unit pairs
typedef std::vector<SOrderUnitPair> OrderUnitPairVector;

#endif // _TTRTS_ORDERUNITPAIR_H_