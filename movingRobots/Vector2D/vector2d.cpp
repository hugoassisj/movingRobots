/**
 * @file vector2d.cpp
 * @brief Implementation of the Vector2D value type.
 */

#include "vector2d.h"

#include <sstream>

Vector2D::Vector2D(int x, int y, int sourceId, int robotId)
    : x(x), y(y), sourceId(sourceId), robotId(robotId)
{}

std::string Vector2D::toString() const
{
    std::ostringstream oss;
    oss << "(x=" << x
        << ", y=" << y
        << ", src=" << sourceId
        << ", robot=" << robotId << ")";
    return oss.str();
}
