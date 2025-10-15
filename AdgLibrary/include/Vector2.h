// ----------------------------------------------------------------
// Vector2 implementation 
// ----------------------------------------------------------------

#ifndef ADG_VECTOR2_H
#define ADG_VECTOR2_H
#include "Scalar.h"
#include <ostream>

namespace adg {

	class Vector2
	{
	public:
		// Member variables
		Scalar x{};
		Scalar y{};

		// Constructors
		Vector2() : x(Scalar(0)), y(Scalar(0)) {}
		explicit Vector2(Scalar in_x, Scalar in_y);

		// Norms
		Scalar squareNorm() const;
		Scalar norm() const;

		// Operators Overload
		// In-place addition
		Vector2& operator += (const Vector2& b);
		// In-place subtraction
		Vector2& operator -= (const Vector2& b);
		// In-place scaling
		Vector2& operator*= (Scalar k);
		// Out-of-place scaling
		Vector2 operator* (Scalar k) const;
		// r-value
		Scalar operator[](int index) const;
		// l-value
		Scalar& operator[](int index);

		// Normalization
		// in-place
		void normalize();
		// out-of-place
		Vector2 normalized() const;

		// Negation
		// in-place
		inline void negate() { (*this) *= -Scalar(1); }
		// out-of-place
		inline Vector2 negated() const { return (*this) * -Scalar(1); }

		// Common unit vectors (like Unity)
		static Vector2 origin() { return Vector2(); }
		static Vector2 right()  { return Vector2(Scalar(1), Scalar(0)); }
		static Vector2 left()   { return Vector2(Scalar(-1), Scalar(0)); }
		static Vector2 up()     { return Vector2(Scalar(0), Scalar(1)); }
		static Vector2 down()   { return Vector2(Scalar(0), Scalar(-1)); }
	};

	// -------------------------------------------------------------------------------
	// Operator Overloads (Non-member)

	// Vector addition
	Vector2 operator+ (const Vector2& a, const Vector2& b);
	// Vector subtraction
	Vector2 operator- (const Vector2& a, const Vector2& b);
	// Commutative closure
	Vector2 operator*(Scalar k, const Vector2& a);
	// Print
	std::ostream& operator<<(std::ostream& os, const Vector2& v);

	// -------------------------------------------------------------------------------
	// Global Vector Operations

	Scalar dot(const Vector2& a, const Vector2& b);

	// something similar to cross in 2D

	inline bool isZero(const Vector2& v) {
		return dot(v, v) < EPSILON;
	}
	inline bool areEqual(const Vector2& v1, const Vector2& v2) {
		return areEqual(v1.x, v2.x) && areEqual(v1.y, v2.y);
	}
	inline Vector2 cleanVector2(const Vector2& v) {
		return Vector2(cleanScalar(v.x), cleanScalar(v.y));
	}

	// Linear interpolation between vectors a and b by factor f
	Vector2 lerp(const Vector2& a, const Vector2& b, Scalar f);

	// normalized interpolation
	Vector2 nlerp(const Vector2& a, const Vector2& b, Scalar f);

	// Reflect vector v around normal vector n
	Vector2 reflect(const Vector2& v, const Vector2& n);

	// Distance between two vectors
	Scalar distance(const Vector2& a, const Vector2& b);

	// Computes the angle between two vectors a and b in radians
	Scalar angleBetweenInRadians(const Vector2& a, const Vector2& b);

	// Computes the angle between two vectors a and b in degrees
	Scalar angleBetweenInDegrees(const Vector2& a, const Vector2& b);

	// Compute the rotated vector around a given angle
	Vector2 rotateVectorByAngle(const Vector2& v, Scalar angleDeg);

	Vector2 computeInterceptionTestWithLine(
		const Vector2& position, 
		const Vector2& dir, 
		const Vector2& point, 
		const Vector2& lineDir);


}

#endif // !ADG_VECTOR2_H