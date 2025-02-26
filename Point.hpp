#pragma once

#include <vector>
#include <iostream>

#include <../submodules/Serialisation/Serialization.hpp>

namespace MyDirektion {
	enum Direktion : unsigned char {
		Up,
		Down,
		Left,
		Right,

		UpLeft,
		UpRight,
		DownLeft,
		DownRight,

		Neutral
	};

	constexpr inline Direktion operator!(const Direktion& d) {
		switch (d) {
		case Up: return Down;
		case Down: return Up;
		case Left: return Right;
		case Right: return Left;
		case UpLeft: return DownRight;
		case UpRight: return DownLeft;
		case DownLeft: return UpRight;
		case DownRight: return UpLeft;
		case Neutral: return Neutral;
		default:
		return Neutral;
		}
	}

	constexpr inline const std::array<std::string_view, size_t(Neutral) + 1> DirektionToString{
		"Up",
		"Down",
		"Left",
		"Right",
		"UpLeft",
		"UpRight",
		"DownLeft",
		"DownRight",
		"Neutral"
	};

	constexpr inline bool contains(const Direktion& d1, const Direktion& d2) {
		if (d1 == d2)return true;
		if (d1 == !d2)return false;
		if (d2 == Up && (d1 == UpRight || d1 == UpLeft))return true;
		if (d2 == Down && (d1 == DownRight || d1 == DownLeft))return true;
		if (d2 == Right && (d1 == UpRight || d1 == DownRight))return true;
		if (d2 == Left && (d1 == UpLeft|| d1 == DownLeft))return true;
		return false;
	}

	constexpr inline Direktion operator+(const Direktion& d1, const Direktion& d2) {
		if (d1 == Neutral) return d2;
		if (d2 == Neutral) return d1;

		if (d1 == !d2) return Neutral;

		if (d1 == d2)return d1;

		if (contains(d1, d2)) return d1;
		if (contains(d2, d1)) return d2;

		switch (d1) {
		case Up:
			switch (d2) {
			case Left: return UpLeft;
			case Right: return UpRight;
			case DownLeft: return Left;
			case DownRight: return Right;
			}
			break;
		case Down:
			switch (d2) {
			case Left: return DownLeft;
			case Right: return DownRight;
			case UpLeft: return Left;
			case UpRight: return Right;
			}
			break;
		case Left:
			switch (d2) {
			case Up: return UpLeft;
			case Down: return DownLeft;
			case UpRight: return Up;
			case DownRight: return Down;
			}
			break;
		case Right:
			switch (d2) {
			case Up: return UpRight;
			case Down: return DownRight;
			case UpLeft: return Up;
			case DownLeft: return Down;
			}
			break;
		case UpLeft:
			switch (d2) {
			case Down: return Left;
			case Right: return Up;
			case DownLeft: return Left;
			case UpRight: return Up;
			}
			break;
		case UpRight:
			switch (d2) {
			case Down: return Right;
			case Left: return Up;
			case DownRight: return Right;
			case UpLeft: return Up;
			}
			break;
		case DownLeft:
			switch (d2) {
			case Up: return Left;
			case Right: return Down;
			case UpLeft: return Left;
			case DownRight: return Down;
			}
			break;
		case DownRight:
			switch (d2) {
			case Up: return Right;
			case Left: return Down;
			case UpRight: return Right;
			case DownLeft: return Down;
			}
			break;
		}
		return Neutral;
	}
}


template <typename T>
class Point {
public:
	T x = 0;
	T y = 0;
public:
	constexpr Point() {}
	constexpr Point(const T& x, const T& y) :x(x), y(y) {}
	constexpr Point(const Point<T>& o) : x(o.x), y(o.y) {}

	template<typename TOther>
	constexpr Point(const Point<TOther>& o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

	/*void Read(std::istream& If) requires SerializationConcepts::Serializable<T> {
		SerializedRead(If,x);
		SerializedRead(If,y);
	}

	void Write(std::ostream& Of) const requires SerializationConcepts::Serializable<T> {
		SerializedWrite(Of, x);
		SerializedWrite(Of, y);
	}*/

	constexpr Point<T>& operator =(const Point<T>& o) {
		x = o.x;
		y = o.y;
		return *this;
	}

	constexpr Point<T>& operator +=(const Point<T>& o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	constexpr Point<T>& operator +=(const MyDirektion::Direktion& d) {
		using namespace MyDirektion;
		if (d == Up)y -= 1;
		if (d == Down)y += 1;
		if (d == Left)x -= 1;
		if (d == Right)x += 1;
		if (d == UpLeft) {
			x -= 1;
			y -= 1;
		}
		if (d == UpRight) {
			x += 1;
			y -= 1;
		}
		if (d == DownLeft) {
			x -= 1;
			y += 1;
		}
		if (d == DownRight) {
			x += 1;
			y += 1;
		}
		return *this;
	}
	constexpr Point<T>& operator -=(const Point<T>& o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}
	constexpr Point<T>& operator -=(const MyDirektion::Direktion& d) {
		return ((*this) += !d);
	}
	constexpr Point<T>& operator *=(const double& d) {
		x *= d;
		y *= d;
		return *this;
	}
	constexpr Point<T>& operator /=(const double& d) {
		x /= d;
		y /= d;
		return *this;
	}

	constexpr Point<T> operator *(const double& d) {
		return Point(static_cast<T>(x * d), static_cast<T>(y * d));
	}

	constexpr Point<T> operator /(const double& d) {
		return Point(static_cast<T>(x / d), static_cast<T>(y / d));
	}

	friend constexpr Point<T> operator * (const double& d, const Point<T>& p) {
		return Point(static_cast<T>(p.x * d), static_cast<T>(p.y * d));
	}

	friend constexpr Point<T> operator / (const double& d, const Point<T>& p) {
		return Point(static_cast<T>(p.x / d), static_cast<T>(p.y / d));
	}

	friend constexpr Point<T> operator +(const Point<T>& p1, const Point<T>& p2) {
		return Point(p1.x + p2.x, p1.y + p2.y);
	}

	friend constexpr Point<T> operator +(const Point<T>& p, const MyDirektion::Direktion& D) {
		return Point<T>(p) += D;
	}

	friend constexpr Point<T> operator +(const MyDirektion::Direktion& D, const Point<T>& p) {
		return p + D;
	}

	friend constexpr Point<T> operator -(const Point<T>& p, const MyDirektion::Direktion& D) {
		return Point<T>(p) -= D;
	}

	friend constexpr Point<T> operator -(const Point<T>& p1, const Point<T>& p2) {
		return Point(p1.x - p2.x, p1.y - p2.y);
	}

	constexpr bool operator ==(const Point<T>& o) const {
		return (x == o.x) && (y == o.y);
	}
	constexpr bool operator != (const Point<T>& o) const {
		return !(*this == o);
	}

	constexpr auto operator<=>(const Point<T>& o) const = default;
};

//Für unordered set
template <typename T>
struct PointHash {
	constexpr std::size_t operator()(const Point<T>& p) const {
		std::hash<T> hasher;
		std::size_t seed = 0;
		seed ^= hasher(p.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= hasher(p.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};