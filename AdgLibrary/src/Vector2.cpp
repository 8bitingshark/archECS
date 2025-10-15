// ----------------------------------------------------------------
// Vector2 implementation
// ----------------------------------------------------------------

#include "Vector2.h"
#include <iostream>

// Constructs a Vector2 from three scalar components
adg::Vector2::Vector2(Scalar in_x, Scalar in_y) : x(in_x), y(in_y) {}

// ---------------------------------------------------------------------------------------------------

// Returns the squared length of the vector (using dot product, otherwise x^2 + y^2)
adg::Scalar adg::Vector2::squareNorm() const {
	return dot(*this, *this);
}

// Returns the Euclidean length (magnitude) of the vector
adg::Scalar adg::Vector2::norm() const {
	return sqrt(squareNorm());
}

// ---------------------------------------------------------------------------------------------------

// In-place addition
adg::Vector2& adg::Vector2::operator+=(const Vector2& b)
{
	x += b.x;
	y += b.y;
	return *this;
}

// In-place subtraction
adg::Vector2& adg::Vector2::operator-=(const Vector2& b)
{
	x -= b.x;
	y -= b.y;
	return *this;
}

// In-place scalar multiplication
adg::Vector2& adg::Vector2::operator*=(Scalar k) {
	x *= k;
	y *= k;
	return *this;
}

// Out-of-place scalar multiplication
adg::Vector2 adg::Vector2::operator*(Scalar k) const {
	return Vector2(x * k, y * k);
}

// Operator []
adg::Scalar adg::Vector2::operator[](int index) const
{
	if (index < 0 || index > 1)
		throw std::out_of_range{ "Vector2: index out of range" };

	// ptr[i] = *(p + i)
	return (&x)[index]; // memory layout dependent
}

adg::Scalar& adg::Vector2::operator[](int index)
{
	if (index < 0 || index > 1)
		throw std::out_of_range{ "Vector2: index out of range" };
	return (&x)[index]; //memory layout dependent
}

// ---------------------------------------------------------------------------------------------------

// Vector addition
adg::Vector2 adg::operator+(const Vector2& a, const Vector2& b) {
	Vector2 ret(a);
	ret += b;
	return ret;
}

// Vector subtraction
adg::Vector2 adg::operator-(const Vector2& a, const Vector2& b) {
	Vector2 ret(a);
	ret -= b;
	return ret;
}

// Scalar multiplication (commutative closure)
adg::Vector2 adg::operator*(Scalar k, const Vector2& a) {
	Vector2 ret(a);
	return a * k;
}

// Print
std::ostream& adg::operator<<(std::ostream& os, const Vector2& v)
{
	return os << "( " << v.x << ", " << v.y << " )";
}

// ---------------------------------------------------------------------------------------------------

// Normalize the vector in-place
void adg::Vector2::normalize() {
	if (isZero((*this)))
		return;
	(*this) *= Scalar(1) / norm();
}

// Returns a normalized copy of the vector
adg::Vector2 adg::Vector2::normalized() const {
	if (isZero((*this)))
		return (*this);
	return (*this) * (Scalar(1) / norm());
}

// ---------------------------------------------------------------------------------------------------
// Global Vector Operations

// Computes the dot product of two vectors
adg::Scalar adg::dot(const Vector2& a, const Vector2& b) {
	return a.x * b.x + a.y * b.y;
}

// Performs linear interpolation between vectors a and b by factor f
adg::Vector2 adg::lerp(const Vector2& a, const Vector2& b, Scalar f) {
	return (Scalar(1) - f) * a + f * b;
}

// normalized interpolation
adg::Vector2 adg::nlerp(const Vector2& a, const Vector2& b, Scalar f)
{
	return lerp(a, b, f).normalized();
}

// Reflects vector v using n as the normal vector
adg::Vector2 adg::reflect(const Vector2& v, const Vector2& n) {
	return v - Scalar(2) * dot(v, n) * n;
}

// Computes the Euclidean distance between vectors a and b
adg::Scalar adg::distance(const Vector2& a, const Vector2& b) {
	return (a - b).norm();
}

// Computes the angle between two vectors a and b in radians
adg::Scalar adg::angleBetweenInRadians(const Vector2& a, const Vector2& b)
{
	Scalar cosAlpha = dot(a, b) / a.norm() * b.norm();
	Scalar angle = std::acos(cosAlpha);

	return angle;
}

// Computes the angle between two vectors a and b in degrees
adg::Scalar adg::angleBetweenInDegrees(const Vector2& a, const Vector2& b)
{
	return toDegrees(angleBetweenInRadians(a, b));
}


adg::Vector2 adg::rotateVectorByAngle(const Vector2& v, Scalar angleDeg)
{
	// anticlockwise rotation is positive
	// v and perp form a basis, 2d coord space
	// scale them using cos (x axis) and sin (y axis) factors and sum resulted vectors
	// to find the rotated one 
	Scalar angleRad = toRadians(angleDeg);
	Vector2 perp = Vector2(-v.y, v.x);
	Vector2 result = cos(angleRad) * v + sin(angleRad) * perp;

	// alternative using a 2d rotation matrix, applied as a linear combination
	Scalar cosAlpha = cos(angleRad);
	Scalar sinAlpha = sin(angleRad);
	Vector2 rot1(cosAlpha, -sinAlpha); // -sinAlpha clockwise rotation, +sinAlpha anticlockwise
	Vector2 rot2(cosAlpha, sinAlpha);
	Vector2 result2 = rot1 * v.x + rot2 * v.y;

	// same method as the first one but with clockwise rotation
	Vector2 leftPerp = Vector2(v.y, -v.x); // not this one otherwise you have flipped basis
	Vector2 result3 = cos(angleRad) * v - sin(angleRad) * perp;

	// so technically to use the leftPerp
	// and rotate clockwise you need to "rotate counterclock"
	// so
	Vector2 result4 = cos(angleRad) * v + sin(angleRad) * leftPerp;

	return result4;
}

adg::Vector2 adg::computeInterceptionTestWithLine(const Vector2& position, const Vector2& dir, const Vector2& point, const Vector2& lineDir)
{
	// check line dir
	Vector2 lineDirRef = lineDir;
	Vector2 pointToPosition = point - position;
	Vector2 pointToPosDir = pointToPosition.normalized();
	
	Scalar dotPosLineDirs = dot(pointToPosDir, lineDir);
	if (dotPosLineDirs < Scalar(0)) // they are not facing the same side
	{
		// flip to evaluate correctly the nearest perpendicular point to line
		lineDirRef.negated();
	}

	// lenght of pointToPosition onto lineDir
	Scalar dotPtPLineDir = fabs(dot(lineDirRef, pointToPosition));
	Vector2 nearestPointToLineFromPosition = point + lineDirRef * dotPtPLineDir;

	// k = d*cosAlpha
	Vector2 nearestToPos = nearestPointToLineFromPosition - position;
	Vector2 nearestToPosDir = nearestToPos.normalized();

	Scalar cosAlpha = dot(nearestToPosDir, dir);
	Vector2 interception{};
	if (!isZero(cosAlpha) && cosAlpha > 0)
	{
		Scalar k = nearestToPos.norm() / cosAlpha;
		interception = position + k * dir;
	}

	return interception;
}
