
#ifndef ADG_SCALAR_H
#define ADG_SCALAR_H
#include <numbers>

namespace adg {

	// define scalar as typedef of float
	typedef float Scalar;

	inline constexpr Scalar EPSILON = Scalar(1e-5);

	inline Scalar sign(Scalar a) { return (a < Scalar(0)) ? Scalar(-1) : (a > Scalar(0) ? Scalar(1) : Scalar(0)); }

	// Converts degrees to radians
	inline Scalar toRadians(Scalar degrees) { return degrees * (std::numbers::pi_v<Scalar> / Scalar(180)); }
	
	// Converts radians to degrees
	inline Scalar toDegrees(Scalar radians) { return radians * (Scalar(180) / std::numbers::pi_v<Scalar>); }

	// Linearly interpolates between a and b by factor f
	inline Scalar lerp(Scalar a, Scalar b, Scalar f) { return a * (Scalar(1) - f) + b * f; }

	// Checks if a scalar value is approximately zero using an epsilon threshold
	inline bool isZero(Scalar a) {
		return a < EPSILON && a > -EPSILON;
	}

	// Checks if two scalars are approximately equal using isZero on their difference
	inline bool areEqual(const Scalar a, const Scalar b) {
		return isZero(a - b);
	}

	inline Scalar cleanScalar(Scalar a) {
		return isZero(a) ? Scalar(0) : a;
	}
}

#endif