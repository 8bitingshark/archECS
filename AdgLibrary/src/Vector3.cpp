// ----------------------------------------------------------------
// Vector3 implementation
// ----------------------------------------------------------------

#include "Vector3.h"

// Constructs a Vector3 from three scalar components
adg::Vector3::Vector3(Scalar in_x, Scalar in_y, Scalar in_z) : x(in_x), y(in_y), z(in_z) {}

// ---------------------------------------------------------------------------------------------------

// Returns the squared length of the vector (using dot product, otherwise x^2 + y^2 + z^2)
adg::Scalar adg::Vector3::squareNorm() const {
	return dot(*this, *this);
}

// Returns the Euclidean length (magnitude) of the vector
adg::Scalar adg::Vector3::norm() const {
	return sqrt(squareNorm());
}

// ---------------------------------------------------------------------------------------------------

// In-place addition
adg::Vector3& adg::Vector3::operator+=(const Vector3& b)
{
	x += b.x;
	y += b.y;
	z += b.z;
	return *this;
}

// In-place subtraction
adg::Vector3& adg::Vector3::operator-=(const Vector3& b)
{
	x -= b.x;
	y -= b.y;
	z -= b.z;
	return *this;
}

// In-place scalar multiplication
adg::Vector3& adg::Vector3::operator*=(Scalar k) {
	x *= k;
	y *= k;
	z *= k;
	return *this;
}

// Out-of-place scalar multiplication
adg::Vector3 adg::Vector3::operator*(Scalar k) const {
	return Vector3(x * k, y * k, z * k);
}

// Operator []
adg::Scalar adg::Vector3::operator[](int index) const
{
	if (index < 0 || index > 2)
		throw std::out_of_range{ "Vector3: index out of range" };

	// ptr[i] = *(p + i)
	return (&x)[index]; // memory layout dependent
}

adg::Scalar& adg::Vector3::operator[](int index)
{
	if (index < 0 || index > 2)
		throw std::out_of_range{ "Vector3: index out of range" };
	return (&x)[index]; //memory layout dependent
}

// ---------------------------------------------------------------------------------------------------

// Vector addition
adg::Vector3 adg::operator+(const Vector3& a, const Vector3& b) {
	Vector3 ret(a);
	ret += b;
	return ret;
}

// Vector subtraction
adg::Vector3 adg::operator-(const Vector3& a, const Vector3& b) {
	Vector3 ret(a);
	ret -= b;
	return ret;
}

// Scalar multiplication (commutative closure)
adg::Vector3 adg::operator*(Scalar k, const Vector3& a) {
	Vector3 ret(a);
	return a * k;
}

// Print
std::ostream& adg::operator<<(std::ostream& os, const Vector3& v)
{
	return os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
}

// ---------------------------------------------------------------------------------------------------

// Normalize the vector in-place
void adg::Vector3::normalize() {
	if (isZero((*this)))
		return;
	(*this) *= 1.0f / norm();
}

// Returns a normalized copy of the vector
adg::Vector3 adg::Vector3::normalized() const {
	if (isZero((*this)))
		return (*this);
	return (*this) * (1.0f / norm());
}

// ---------------------------------------------------------------------------------------------------
// Global Vector Operations

// Computes the dot product of two vectors
adg::Scalar adg::dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Computes the cross product of two vectors
adg::Vector3 adg::cross(const Vector3& a, const Vector3& b) {
	return Vector3(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

// Performs linear interpolation between vectors a and b by factor f
adg::Vector3 adg::lerp(const Vector3& a, const Vector3& b, Scalar f) {
	return (Scalar(1) - f) * a + f * b;
}

// normalized interpolation
adg::Vector3 adg::nlerp(const Vector3& a, const Vector3& b, Scalar f)
{
	return lerp(a, b, f).normalized();
}

// Reflects vector v using n as the normal vector
adg::Vector3 adg::reflect(const Vector3& v, const Vector3& n) {
	return v - Scalar(2) * dot(v, n) * n;
}

// Computes the Euclidean distance between vectors a and b
adg::Scalar adg::distance(const Vector3& a, const Vector3& b) {
	return (a - b).norm();
}

// Computes the angle between two vectors a and b in radians
adg::Scalar adg::angleBetweenInRadians(const Vector3& a, const Vector3& b)
{
	// dot(a,b) = ||a||*||b||*cos(alpha)
	Scalar cosAlpha = dot(a, b) / a.norm() * b.norm();

	// ||cross(a,b)|| = ||a||*||b||*sin(alpha)
	Scalar sinAlpha = cross(a, b).norm() / a.norm() * b.norm();

	// atan2(sin(alpha), cos(alpha)) = alpha
	return std::atan2(sinAlpha, cosAlpha);
}

// Computes the angle between two vectors a and b in degrees
adg::Scalar adg::angleBetweenInDegrees(const Vector3& a, const Vector3& b)
{
	return toDegrees(angleBetweenInRadians(a, b));
}

// Compute the rotated vector around a given axis and angle in degrees
adg::Vector3 adg::rotateVectorAroundAxis(const Vector3& v, const Vector3& axis, Scalar angleDeg)
{
	// from the axis I need to find the perpendicular component of the vector
	// compute the rotation with the perp component and then reconstruct the so rotated vector
	// To do this, first compute the parallel component of the vector
	// vp: project the vector onto the axis and find its extension
	Vector3 vp = dot(axis, v) * axis;

	// since v = vp + vo
	// vo = v - vp
	// vo = perpendicular component
	Vector3 vo = v - vp;

	// Find a perpendicular vector to the axis, w
	// and form a 2D space with vo and w (note that aren't necessarily unit length)
	// w is perpendicular to both vp and vo. And has the same length of vo!
	// axis x vo = axis x (v - vp) = axis x v - axis x vp = axis x v - 0
	Vector3 w = cross(axis, v);

	// linear combination to find the rotated vo
	// vo and w basis vectors
	Scalar angleRad = toRadians(angleDeg);
	Vector3 vorotated = cos(angleRad) * vo + sin(angleRad) * w;

	// combine the result to get the rotated vector
	return vorotated + vp;
}