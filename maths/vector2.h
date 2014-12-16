#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "mathtypes.h"

struct uvector2;

struct vector2
{
    vector2() : x (0), y (0) {}
    vector2( coord_t _x, coord_t _y )
    : x(_x)
    , y(_y)
    {}

	coord_t x;
	coord_t y;

    inline vector2 operator-() const { return { (coord_t)-x, (coord_t)-y }; }

    inline operator uvector2() const;

    // vec
    inline vector2 operator+(const vector2& rhs) const { return { (coord_t)(rhs.x + x) , (coord_t)(rhs.y + y) }; }
    inline vector2 operator-(const vector2& rhs) const { return *this + (-rhs); }

    inline const vector2& operator+=(const vector2& rhs) { *this = *this+rhs; return *this; }
    inline const vector2& operator-=(const vector2& rhs) { return *this+=(-rhs); }

    inline bool operator==(const vector2& rhs) const { return ( rhs.x == x) && ( rhs.y == y); }
    inline bool operator!=(const vector2& rhs) const { return !(*this==rhs); }
};

struct uvector2
{
    uvector2() : x (0), y (0) {}
    uvector2( ucoord_t _x, ucoord_t _y )
    : x(_x)
    , y(_y)
    {}

	ucoord_t x;
	ucoord_t y;

    // Implicit conversion to vector 2 if needed
    inline operator vector2() const { return { (coord_t)x, (coord_t)y }; }

    inline uvector2 operator-() const { return { (ucoord_t)-x, (ucoord_t)-y }; }

    // uvec
    inline uvector2 operator+(const uvector2& rhs) const { return { (ucoord_t)(rhs.x + x) , (ucoord_t)(rhs.y + y) }; }
    inline uvector2 operator-(const uvector2& rhs) const { return *this + (-rhs); }

    inline const uvector2& operator+=(const uvector2& rhs) { *this = *this+rhs; return *this; }
    inline const uvector2& operator-=(const uvector2& rhs) { return *this+=(-rhs); }

    inline bool operator==(const uvector2& rhs) const { return ( rhs.x == x) && ( rhs.y == y); }
    inline bool operator!=(const uvector2& rhs) const { return !(*this==rhs); }
};

inline vector2::operator uvector2() const { return { (ucoord_t)x, (ucoord_t)y }; }

inline vector2 vecFromDir( dir_t dir )
{
    switch( dir )
    {
    case dir_t::N:
        return { 0,1 };

    case dir_t::E:
        return { 1,0 };

    case dir_t::S:
        return { 0,-1 };

    case dir_t::W:
        return { -1,0 };

    default:
        return { 0,0 };
    }
}

#endif //_VECTOR2_H_
