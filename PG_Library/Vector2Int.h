#pragma once

class Vector2Int
{
public:
	int x = 0, y = 0;

	Vector2Int() = default;
	Vector2Int(int sameValue);
	Vector2Int(int x, int y);

	Vector2Int	operator+(const Vector2Int& other) const;
	Vector2Int	operator-(const Vector2Int& other) const;
	Vector2Int	operator*(const Vector2Int& other) const;
	Vector2Int	operator/(const Vector2Int& other) const;

	Vector2Int& operator+=(const Vector2Int& other);
	Vector2Int& operator-=(const Vector2Int& other);
	Vector2Int& operator*=(const Vector2Int& other);
	Vector2Int& operator/=(const Vector2Int& other);

	//Boolean Equals Operators
	bool		operator==(const Vector2Int& other) const;
	bool		operator!=(const Vector2Int& other) const;

	bool OutSide(const Vector2Int& other) const;
	
	//     Shorthand for writing Vector2Int (0, -1).
	static const Vector2Int down;
	//     Shorthand for writing Vector2Int (0, 1).
	static const Vector2Int up;
	//     Shorthand for writing Vector2Int (1, 1).
	static const Vector2Int one;
	//     Shorthand for writing Vector2Int (0, 0).
	static const Vector2Int zero;
	//     Shorthand for writing Vector2Int (-1, 0).
	static const Vector2Int left;
	//     Shorthand for writing Vector2Int (1, 0).
	static const Vector2Int right;
};

