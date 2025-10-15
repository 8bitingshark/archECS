// ----------------------------------------------------------------
// Vector3 implementation
// ----------------------------------------------------------------

#ifndef ADG_VECTOR3_H
#define ADG_VECTOR3_H
#include "Scalar.h"
#include <iostream>
#include <math.h>

namespace adg {

	// 3D vector class using Scalar (typedef of float)
	class Vector3 {

	public:

		// Member variables
		Scalar x{};
		Scalar y{};
		Scalar z{};

		// Constructors
		Vector3() : x(Scalar(0)), y(Scalar(0)), z(Scalar(0)) {}
		explicit Vector3(Scalar in_x, Scalar in_y, Scalar in_z);


		// Norms
		Scalar squareNorm() const;
		Scalar norm() const;


		// Operators Overload
		// In-place addition
		Vector3& operator += (const Vector3& b);
		// In-place subtraction
		Vector3& operator -= (const Vector3& b);
		// In-place scaling
		Vector3& operator*= (Scalar k);
		// Out-of-place scaling
		Vector3 operator* (Scalar k) const;
		// r-value
		Scalar operator[](int index) const;
		// l-value
		Scalar& operator[](int index);

		// Normalization
		// in-place
		void normalize();
		// out-of-place
		Vector3 normalized() const;

		// Negation
		// in-place
		inline void negate() { (*this) *= -Scalar(1); }
		// out-of-place
		inline Vector3 negated() const { return (*this) * -Scalar(1); }

		// Common unit vectors (like Unity)
		static Vector3 origin()  { return Vector3(); }
		static Vector3 right()   { return Vector3(Scalar(1),  Scalar(0),  Scalar(0));  }
		static Vector3 left()    { return Vector3(Scalar(-1), Scalar(0),  Scalar(0));  }
		static Vector3 forward() { return Vector3(Scalar(0),  Scalar(0),  Scalar(1));  }
		static Vector3 back()    { return Vector3(Scalar(0),  Scalar(0),  Scalar(-1)); }
		static Vector3 up()      { return Vector3(Scalar(0),  Scalar(1),  Scalar(0));  }
		static Vector3 down()    { return Vector3(Scalar(0),  Scalar(-1), Scalar(0));  }
	};


	// Special constant: Vector indicating a missed value
	const Vector3 MISSED(NAN, NAN, NAN);

	// -------------------------------------------------------------------------------
	// Operator Overloads (Non-member)
	
	// Vector addition
	Vector3 operator+ (const Vector3& a, const Vector3& b);
	// Vector subtraction
	Vector3 operator- (const Vector3& a, const Vector3& b);
	// Commutative closure
	Vector3 operator*(Scalar k, const Vector3& a);
	// Print
	std::ostream& operator<<(std::ostream& os, const Vector3& v);

	// -------------------------------------------------------------------------------
	// Global Vector Operations

	Scalar dot(const Vector3& a, const Vector3& b);
	Vector3 cross(const Vector3& a, const Vector3& b);

	inline bool isZero(const Vector3& v) {
		return dot(v, v) < EPSILON;
	}
	inline bool areEqual(const Vector3& v1, const Vector3& v2) {
		return areEqual(v1.x, v2.x) && areEqual(v1.y, v2.y) && areEqual(v1.z, v2.z);
	}
	inline Vector3 cleanVector3(const Vector3& v) {
		return Vector3(cleanScalar(v.x), cleanScalar(v.y), cleanScalar(v.z));
	}

	// Linear interpolation between vectors a and b by factor f
	Vector3 lerp(const Vector3& a, const Vector3& b, Scalar f);

	// normalized interpolation
	Vector3 nlerp(const Vector3& a, const Vector3& b, Scalar f);

	// Reflect vector v around normal vector n
	Vector3 reflect(const Vector3& v, const Vector3& n);

	// Distance between two vectors
	Scalar distance(const Vector3& a, const Vector3& b);

	// Computes the angle between two vectors a and b in radians
	Scalar angleBetweenInRadians(const Vector3& a, const Vector3& b);

	// Computes the angle between two vectors a and b in degrees
	Scalar angleBetweenInDegrees(const Vector3& a, const Vector3& b);

	// Compute the rotated vector around a given axis and angle in degrees
	Vector3 rotateVectorAroundAxis(const Vector3& v, const Vector3& axis, Scalar angleDeg);

} // namespace adg

#endif // ADG_VECTOR3_H