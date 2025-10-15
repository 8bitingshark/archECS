// ----------------------------------------------------------------
// Quaternion implementation
// ----------------------------------------------------------------

#include "Quaternion.h"
#include "Matrix.h"
#include <iostream>
#include <array>
#include <algorithm>

// -----------------------------------------------------------------------------
// Constructors

adg::Quaternion::Quaternion() {}

adg::Quaternion::Quaternion(Scalar in_w) : im(), w(in_w) {}

adg::Quaternion::Quaternion(const Vector3& in_im, Scalar in_w) : im(in_im), w(in_w) {}

adg::Quaternion::Quaternion(Scalar in_x, Scalar in_y, Scalar in_z, Scalar in_w) : im(in_x, in_y, in_z), w(in_w) {}

// -------------------------------------------------------------------------------
// Conjugation

void adg::Quaternion::conjugate()
{
	im.negate();
}

adg::Quaternion adg::Quaternion::conjugated() const
{
	return Quaternion(im.negated(), w);
}

// -------------------------------------------------------------------------------
// Norms

// square magnitude of q = dot(q,q) = q * q.conjugated()
adg::Scalar adg::Quaternion::squareNorm() const {

	return dot(*this, *this);
}

adg::Scalar adg::Quaternion::norm() const {
	return sqrt(squareNorm());
}

// -------------------------------------------------------------------------------
// Normalization

// In-place
void adg::Quaternion::normalize() {
	Scalar n = norm();
	if (n > Scalar(0)) {
		*this = *this / n;
	}
	else {
		*this = Quaternion();
	}
}

// Out-of-place
adg::Quaternion adg::Quaternion::normalized() const {

	return Quaternion((*this) / norm());
}

// -------------------------------------------------------------------------------
// Negation

void adg::Quaternion::negate(){ 
	im.negate(); 
	w = -w; 
}

adg::Quaternion adg::Quaternion::negated() const {

	return Quaternion(im.negated(), -w);
}

// -------------------------------------------------------------------------------
// Logarithm

void adg::Quaternion::log()
{
	Scalar hAngle = Scalar(2) * atan2(im.norm(), w);
	w = Scalar(0);
	im.normalize();
	im *= hAngle;
}

adg::Quaternion adg::Quaternion::log() const
{
	Scalar hAngle = Scalar(2) * atan2(im.norm(), w);

	return Quaternion(im.normalized() * hAngle, Scalar(0));
}

// -------------------------------------------------------------------------------
// Exponentiation

void adg::Quaternion::exponentiate(Scalar t)
{
	// prevent the division by zero
	if (std::fabs(w) < Scalar(.99999))
		return;

	// extract the half angle
	Scalar hAngle = Scalar(2) * atan2(im.norm(), w);

	// compute the new half angle
	Scalar nhAngle = hAngle * t;

	// update the quaternion
	w = cos(nhAngle);
	Scalar mult = sin(nhAngle) / sin(hAngle);
	im *= mult;
}

adg::Quaternion adg::Quaternion::exponentiated(Scalar t) const
{
	if (std::fabs(w) < Scalar(.99999))
		return Quaternion();

	Scalar hAngle = Scalar(2) * atan2(im.norm(), w);
	Scalar nhAngle = hAngle * t;
	Scalar mult = sin(nhAngle) / sin(hAngle);

	return Quaternion(im * mult, cos(nhAngle));
}

// -------------------------------------------------------------------------------
// Operators Overload (Members)

// In-place
adg::Quaternion& adg::Quaternion::operator+=(const Quaternion& p)
{
	im += p.im;
	w  += p.w;
	return *this;
}

adg::Quaternion& adg::Quaternion::operator*=(Scalar s){

	im *= s;
	w  *= s;
	return *this;
}

adg::Quaternion& adg::Quaternion::operator*=(const Quaternion& p) {

	(*this) = (*this) * p;
	return *this;
}

adg::Quaternion& adg::Quaternion::operator/(Scalar s){

	im.x /= s;
	im.y /= s;
	im.z /= s;
	w    /= s;
	return *this;
}

// Out-of-place
adg::Quaternion adg::Quaternion::operator*(adg::Scalar s) const {

	return Quaternion(im * s, w * s);
}

adg::Quaternion adg::Quaternion::operator/(Scalar s) const
{
	return Quaternion(im.x/s, im.y/s, im.z/s, w / s);
}


// -------------------------------------------------------------------------------
// Operator Overloads (Non-member)

adg::Quaternion adg::operator+(const Quaternion& q, const Quaternion& p)
{
	return Quaternion(q.im + p.im, q.w + p.w);
}

// Quaternion multiplication (no-commutative)
adg::Quaternion adg::operator*(const Quaternion& q, const Quaternion& p)
{
	return Quaternion(q.w * p.im + q.im * p.w + cross(q.im, p.im), q.w * p.w - dot(q.im, p.im));
}

std::ostream& adg::operator<<(std::ostream& os, const Quaternion& q)
{
	return os << "(" 
			  << q.im.x << " i, "
			  << q.im.y << " j, "
			  << q.im.z << " k, "
			  << q.w
			  << ")";
}


// -------------------------------------------------------------------------------
// Global Quaternion Operations

// dot product, distance in 4D with unit quaternions
adg::Scalar adg::dot(const Quaternion& q, const Quaternion& p)
{
	return dot(q.im, p.im) + q.w*p.w;
}

// linear interpolation
// quaternion doesn't remain unitary
adg::Quaternion adg::lerp(const Quaternion& q, const Quaternion& p, Scalar f)
{
	return (q*(Scalar(1) - f) + p*f);
}

adg::Quaternion adg::nlerp(const Quaternion& q, const Quaternion& p, Scalar f)
{
	return lerp(q,p,f).normalized();
}

bool adg::isUnitary(const Quaternion& q)
{
	return areEqual(q.squareNorm(), Scalar(1));
}

adg::Quaternion adg::cleanQuaternion(const Quaternion& q)
{
	return Quaternion(cleanVector3(q.im), cleanScalar(q.w));
}


// -------------------------------------------------------------------------------
// Unit Quaternion Operations

adg::Quaternion adg::mix(const Quaternion& q, const Quaternion& p, Scalar f)
{
	// shortest path: p or -p is closer to q?
	// take the dot product of q and p if it is positive interpolate with p
	// otherwise interpolate with -p
	Scalar s = sign(dot(q, p));
	Quaternion b = q * (Scalar(1) - f) + p * f * s;
	return b.normalized();
}

// spherical linear interpolation
// using similar triangles
adg::Quaternion adg::slerp(const Quaternion& q, const Quaternion& p, Scalar f)
{
	// shortest path
	Scalar cosAlpha = dot(q, p);
	Scalar cosSign = sign(cosAlpha);

	cosAlpha *= cosSign;

	if (cosAlpha > Scalar(0.99999))
	{
		// protect against division by zero
		// use linear interpolation
		std::cout << "use mix not slerp" << std::endl;
		return mix(q, p, f);
	}

	Scalar sinAlpha = sqrt(Scalar(1) - dot(p, q) * dot(p, q));
	Scalar alpha = atan2(sinAlpha, cosAlpha);
	Scalar oneOverSinAlpha = Scalar(1) / sinAlpha;

	Scalar af = std::sin(alpha * (Scalar(1) - f)) * oneOverSinAlpha;
	Scalar bf = std::sin(alpha * f) * oneOverSinAlpha;

	return Quaternion(q * af + p * cosSign * bf);
}

adg::Vector3 adg::applyRotation(const Vector3& v, const Quaternion& q)
{
	Quaternion result = q * Quaternion(v, Scalar(0)) * q.conjugated();

	return result.im;
}

adg::Vector3 adg::applyRotationOptimized(const Vector3& v, const Quaternion& q)
{
	/*
	* 
	* Expansion:
	* 
	* Quaternion r = q * Quaternion(v, 0.0f) * q.conjugated();
	* Quaternion r = Quaternion(q.w * v + cross(q.im, v), - dot(q.im, v)) * q.conjugated();
	* Quaternion r = Quaternion(q.im * dot(q.im,v) + q.w * q.w * v + q.w * cross(q.im, v) + cross(q.w * v + cross(q.im, v), -q.im),
	                            -dot(q.im,v) * q.w - dot(q.w * v + cross(q.im, v), -q.im));
	* 
	* I need to return only the imaginary part so,
	* but first I can use these formulas
	* 
	* (P ± R) × Q = P × Q ± R × Q
	* (aP)×Q = a(P×Q)
	* P x Q = - (Q x P)
	* P x (Q x P) = P x Q x P = P^2Q - dot(P,Q)P  (to achieve this, for example I can compute x-component by first evaluating the cross
	*											   product, then ± Px^2Qx to get P^2Qx - dot(P,Q)Px).
	* 
	* to rewrite cross(q.w * v + cross(q.im, v), -q.im) 
	* again:
	* = cross(q.w * v, -q.im) + cross(cross(q.im, v), -q.im)
	* = - cross(q.w * v, q.im) - cross(cross(q.im,v), q.im)
	* = + q.w * cross(q.im, v) - cross(cross(q.im,v), q.im)
	* = + q.w * cross(q.im, v) - dot(q.im,q.im) * v + dot(q.im,v) * q.im
	* 
	* the final result:
	* Vector3 result = 2 * q.im * dot(q.im,v) + q.w * q.w * v + 2 * q.w * cross(q.im, v) - dot(q.im,q.im) * v;
	*/
	
	return Scalar(2) * dot(q.im, v) * q.im
		   + (q.w * q.w) * v
		   + Scalar(2) * q.w * cross(q.im, v)
		   - (dot(q.im, q.im) * v);
}

// assuming the quaternion is already unitary
adg::Quaternion adg::inverseQuaternion(const Quaternion& q)
{
	return q.conjugated();
}

// -------------------------------------------------------------------------------
// Notation Conversions:

// Quaternion <-> AxisAngle
adg::Quaternion adg::axisAngleToQuaternion(const Vector3& in_axis, Scalar in_degrees)
{
	Scalar ar = toRadians(in_degrees);
	// just to be sure
	Vector3 nAxis = in_axis.normalized();
	return Quaternion(sin(ar / Scalar(2)) * nAxis, cos(ar / Scalar(2)));
}

adg::Vector3 adg::quaternionToAxisAngle(const Quaternion& q)
{
	Scalar sinAngleMid = (q.im).norm();
	Scalar angle = Scalar(2) * atan2(sinAngleMid, q.w);
	Vector3 axis = (q.im).normalized();

	return Vector3(axis * angle);
}

// Quaternion <-> EulerAngles
adg::Quaternion adg::eulerToQuaternion(Scalar in_pitchDeg, Scalar in_yawDeg, Scalar in_rollDeg)
{
	Quaternion qRoll = axisAngleToQuaternion(Vector3::forward(), in_rollDeg);	     
	Quaternion qPitch = axisAngleToQuaternion(Vector3::right(), in_pitchDeg);       
	Quaternion qYaw = axisAngleToQuaternion(Vector3::up(), in_yawDeg);			

	// Z → X → Y (roll  Z, pitch  X, yaw Y)
	return ((qYaw * qPitch) * qRoll).normalized();
}

adg::Quaternion adg::eulerToQuaternion_v2(Scalar in_pitchDeg, Scalar in_yawDeg, Scalar in_rollDeg)
{
	// Z → X → Y
	Scalar hr = toRadians(in_rollDeg) * Scalar(0.5);
	Scalar hp = toRadians(in_pitchDeg) * Scalar(0.5);
	Scalar hy = toRadians(in_yawDeg) * Scalar(0.5);

	Scalar sR = sin(hr), cR = cos(hr);
	Scalar sP = sin(hp), cP = cos(hp);
	Scalar sY = sin(hy), cY = cos(hy);

	Scalar qx = cY * sP * cR + sY * cP * sR;
	Scalar qy = sY * cP * cR - cY * sP * sR;
	Scalar qz = cY * cP * sR - sY * sP * cR;
	Scalar qw = cY * cP * cR + sY * sP * sR;

	return Quaternion(qx, qy, qz, qw).normalized();
}

adg::Vector3 adg::quaternionToEulerAngles(const Quaternion& q)
{
	// convert an object-to-world quaternion to euler angles
	// to do the opposite use the conjugate

	Quaternion qn{};
	if (!isUnitary(q))
		qn = q.normalized();
	else
		qn = q;

	Scalar roll{}, pitch{}, yaw{};

	// extract sin(Pitch)
	Scalar sP = -Scalar(2) * (qn.im.y * qn.im.z - qn.w * qn.im.x);

	// Check gimbal lock
	if (fabs(sP) > Scalar(0.999999))
	{
		// looking straight up or down
		pitch = std::numbers::pi_v<Scalar> * Scalar(0.5) * sP;

		// slam roll to zero and compute yaw
		roll = Scalar(0);
		yaw  = atan2(-qn.im.x * qn.im.z + qn.w * qn.im.y, 
			         Scalar(0.5) - qn.im.y * qn.im.y - qn.im.z * qn.im.z);
	}
	else
	{
		pitch = asin(sP);
		yaw   = atan2(qn.im.x * qn.im.z + qn.w * qn.im.y, 
			          Scalar(0.5) - qn.im.x * qn.im.x - qn.im.y * qn.im.y);
		roll  = atan2(qn.im.x * qn.im.y + qn.w * qn.im.z,
			          Scalar(0.5) - qn.im.x * qn.im.x - qn.im.z * qn.im.z);
	}

	return Vector3(toDegrees(pitch), toDegrees(yaw), toDegrees(roll));
}

// Quaternion <-> Matrix3x3
adg::Quaternion adg::matrix3x3ToQuaternion(const Matrix3x3& mat)
{
	// determine which of w, x, y, z has the largest absolute value
	// tr(mat) = 4w^2 - 1
	Scalar fourWSquaredMinusOne = mat(0, 0) + mat(1, 1) + mat(2, 2);
	Scalar fourXSquaredMinusOne = mat(0, 0) - mat(1, 1) - mat(2, 2);
	Scalar fourYSquaredMinusOne = mat(1, 1) - mat(0, 0) - mat(2, 2);
	Scalar fourZSquaredMinusOne = mat(2, 2) - mat(0, 0) - mat(1, 1);
	
	std::array<Scalar, 4> v = {
	fourWSquaredMinusOne,
	fourXSquaredMinusOne,
	fourYSquaredMinusOne,
	fourZSquaredMinusOne
	};

	// find max and index
	auto it = std::max_element(v.begin(), v.end());
	int biggestIndex = static_cast<int>(std::distance(v.begin(), it));
	Scalar fourBiggestSquaredMinusOne = *it;

	Scalar biggestVal = sqrt(fourBiggestSquaredMinusOne + Scalar(1.0)) * Scalar(0.5);
	Scalar mult = Scalar(0.25) / biggestVal;

	// compute quaternion valuse
	Scalar w{}, x{}, y{}, z{};
	
	switch (biggestIndex)
	{
	case 0:
		w = biggestVal;
		x = (mat(2, 1) - mat(1, 2)) * mult;
		y = (mat(0, 2) - mat(2, 0)) * mult;
		z = (mat(1, 0) - mat(0, 1)) * mult;
		break;
	case 1:
		w = (mat(2, 1) - mat(1, 2)) * mult;
		x = biggestVal;
		y = (mat(1, 0) - mat(0, 1)) * mult;
		z = (mat(0, 2) - mat(2, 0)) * mult;
		break;
	case 2:
		w = (mat(0, 2) - mat(2, 0)) * mult;
		x = (mat(1, 0) - mat(0, 1)) * mult;
		y = biggestVal;
		z = (mat(2, 1) - mat(1, 2)) * mult;
		break;
	case 3:
		w = (mat(1, 0) - mat(0, 1)) * mult;
		x = (mat(0, 2) - mat(2, 0)) * mult;
		y = (mat(2, 1) - mat(1, 2)) * mult;
		z = biggestVal;
		break;
	}

	return Quaternion(x, y, z, w);
}

adg::Matrix3x3 adg::quaternionToMatrix3x3(const Quaternion& q)
{
	// other way is to expand qvq.conjugated()

	// build a matrix3x3 from axis and angle
	// then manipulate the matrix in a form that we can use
	// w = cos(θ/2),
	// x = nx sin(θ / 2),
	// y = ny sin(θ / 2),
    // z = nz sin(θ / 2).
	// at the end:

	adg::Vector3 v1(Scalar(1) - Scalar(2) * q.im.y * q.im.y - Scalar(2) * q.im.z * q.im.z,
		Scalar(2) * q.im.x * q.im.y + Scalar(2) * q.w * q.im.z,
		Scalar(2) * q.im.x * q.im.z - Scalar(2) * q.w * q.im.y);

	adg::Vector3 v2(Scalar(2) * q.im.x * q.im.y - Scalar(2) * q.w * q.im.z,
		Scalar(1) - Scalar(2) * q.im.x * q.im.x - Scalar(2) * q.im.z * q.im.z,
		Scalar(2) * q.im.y * q.im.z + Scalar(2) * q.w * q.im.x);

	adg::Vector3 v3(Scalar(2) * q.im.x * q.im.z + Scalar(2) * q.w * q.im.y,
		Scalar(2) * q.im.y * q.im.z - Scalar(2) * q.w * q.im.x,
		Scalar(1) - Scalar(2) * q.im.x * q.im.x - Scalar(2) * q.im.y * q.im.y);

	return Matrix3x3(v1, v2, v3);
}

/*
Matrix3x3 Quaternion::toMatrix3x3() const
{
	Quaternion q = *this;
	if (!isUnit(q))
	{
		q.normalize();
	}

	Vector3D c0 = (q * Quaternion(Vector3D(1.f, 0.f, 0.f), 0.f) * q.conjugated()).im;
	Vector3D c1 = (q * Quaternion(Vector3D(0.f, 1.f, 0.f), 0.f) * q.conjugated()).im;
	Vector3D c2 = (q * Quaternion(Vector3D(0.f, 0.f, 1.f), 0.f) * q.conjugated()).im;

	return Matrix3x3(c0, c1, c2);
}

*/