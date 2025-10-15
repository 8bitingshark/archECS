// ----------------------------------------------------------------
// Quaternion implementation
// ----------------------------------------------------------------

#ifndef ADG_QUATERNION_H
#define ADG_QUATERNION_H
#include "Vector3.h"

namespace adg {

	class Matrix3x3;

	class Quaternion {

	public:

		// Members
		Vector3 im{};
		Scalar  w{};

		// Constructors
		Quaternion();
		explicit Quaternion(Scalar in_w);
		Quaternion(const Vector3& in_im, Scalar in_w);
		Quaternion(Scalar in_x, Scalar in_y, Scalar in_z, Scalar in_w);

		// Conjugation
		void conjugate();
		Quaternion conjugated() const;

		// Norms
		Scalar squareNorm() const;
		Scalar norm() const;

		// Operators Overload (Members)
		// In-place
		Quaternion& operator+=(const Quaternion& p);
		Quaternion& operator*=(Scalar s);
		Quaternion& operator*=(const Quaternion& p);
		Quaternion& operator/(Scalar s);
		// Out-of-place
		Quaternion operator*(Scalar s) const;
		Quaternion operator/(Scalar s) const;

		// Normalization
		void normalize();
		Quaternion normalized() const;

		// Negation
		void negate();
		Quaternion negated() const;

		// Logarithm
		void log();
		Quaternion log() const;

		// Exponentiation
		void exponentiate(Scalar t);
		Quaternion exponentiated(Scalar t) const;

		// Identity
		static Quaternion identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }
	};

	// -------------------------------------------------------------------------------
	// Operator Overloads (Non-member)
	// Quaternion summing
	Quaternion operator+(const Quaternion& q, const Quaternion& p);

	// Quaternion multiplication
	Quaternion operator*(const Quaternion& q, const Quaternion& p);

	// print
	std::ostream& operator<<(std::ostream& os, const Quaternion& q);

	// -------------------------------------------------------------------------------
	// Global Quaternion Operations

	Scalar dot(const Quaternion& q, const Quaternion& p);

	inline bool areEqual(const Quaternion& q, const Quaternion& p) {
		return areEqual(q.im, p.im) && areEqual(q.w, p.w);
	}

	Quaternion lerp(const Quaternion& q, const Quaternion& p, Scalar f);
	Quaternion nlerp(const Quaternion& q, const Quaternion& p, Scalar f);

	bool isUnitary(const Quaternion& q);
	Quaternion cleanQuaternion(const Quaternion& q);

	// -------------------------------------------------------------------------------
	// Unit Quaternion Operations

	Quaternion mix(const Quaternion& q, const Quaternion& p, Scalar f);
	Quaternion slerp(const Quaternion& q, const Quaternion& p, Scalar f);

	Vector3 applyRotation(const Vector3& v, const Quaternion& q);
	Vector3 applyRotationOptimized(const Vector3& v, const Quaternion& q);

	Quaternion inverseQuaternion(const Quaternion& q);

	inline bool areRotationsEquivalent(const Quaternion& q, const Quaternion& p) {
		return areEqual(q, p) || areEqual(q, p.negated());
	}
	
	// -------------------------------------------------------------------------------
	// Notation Conversions:

	// Quaternion <-> AxisAngle
	Quaternion axisAngleToQuaternion(const Vector3& in_axis, Scalar in_degrees);
	Vector3    quaternionToAxisAngle(const Quaternion& q);

	// Quaternion <-> EulerAngles
	Quaternion eulerToQuaternion(Scalar in_pitchDeg, Scalar in_yawDeg, Scalar in_rollDeg);
	Quaternion eulerToQuaternion_v2(Scalar in_pitchDeg, Scalar in_yawDeg, Scalar in_rollDeg);
	Vector3    quaternionToEulerAngles(const Quaternion& q);

	// Quaternion <-> Matrix3x3
	Quaternion matrix3x3ToQuaternion(const Matrix3x3& mat);
	Matrix3x3  quaternionToMatrix3x3(const Quaternion& q);
}


#endif // !ADG_QUATERNION_H