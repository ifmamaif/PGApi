#include "pch.h"
#include "Vector2Int.h"

const Vector2Int Vector2Int::down(0,-1);
const Vector2Int Vector2Int::up(0,1);
const Vector2Int Vector2Int::one(1,1);
const Vector2Int Vector2Int::zero(0,0);
const Vector2Int Vector2Int::left(-1,0);
const Vector2Int Vector2Int::right(1,0);

Vector2Int::Vector2Int(int sameValue)
	:x(sameValue)
	,y(sameValue)
{
}

Vector2Int::Vector2Int(int x, int y)
	: x(x)
	, y(y)
{
}

Vector2Int Vector2Int::operator+(const Vector2Int& other) const
{
	return Vector2Int(x + other.x, y + other.y);
}

Vector2Int Vector2Int::operator-(const Vector2Int& other) const
{
	return Vector2Int(x - other.x, y - other.y);
}

Vector2Int Vector2Int::operator*(const Vector2Int& other) const
{
	return Vector2Int(x * other.x, y * other.y);
}

Vector2Int Vector2Int::operator/(const Vector2Int& other) const
{
	return Vector2Int(x / other.x, y / other.y);
}

Vector2Int& Vector2Int::operator+=(const Vector2Int& other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vector2Int& Vector2Int::operator-=(const Vector2Int& other)
{
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

Vector2Int& Vector2Int::operator*=(const Vector2Int& other)
{
	this->x *= other.x;
	this->y *= other.y;
	return *this;
}

Vector2Int& Vector2Int::operator/=(const Vector2Int& other)
{
	this->x /= other.x;
	this->y /= other.y;
	return *this;
}

bool Vector2Int::operator==(const Vector2Int& other) const
{
	return this->x == other.x && this->y == other.y;
}

bool Vector2Int::operator!=(const Vector2Int& other) const
{
	return !this->operator==(other);
}

bool Vector2Int::OutSide(const Vector2Int& other) const
{
	return x >= other.x || x < -other.x ||
		y >= other.y || y < -other.y;
}