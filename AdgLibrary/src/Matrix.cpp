// ----------------------------------------------------------------
// Matrix3x3 implementation
// ----------------------------------------------------------------

#include "Matrix.h"
#include <iostream>

// -------------------------------------------------------------------------------
// Constructors

adg::Matrix3x3::Matrix3x3() {}

adg::Matrix3x3::Matrix3x3(Vector3 in_a, Vector3 in_b, Vector3 in_c) : a(in_a), b(in_b), c(in_c) {}

// -------------------------------------------------------------------------------
// Operator overloads (members)

adg::Scalar adg::Matrix3x3::operator()(int row, int col) const
{
	if (row < 0 || row > 2 || col < 0 || col > 2)
		throw std::out_of_range{ "Matrix3x3: index out of range" };

	return (&a)[col][row];
}

adg::Scalar& adg::Matrix3x3::operator()(int row, int col)
{
	if (row < 0 || row > 2 || col < 0 || col > 2)
		throw std::out_of_range{ "Matrix3x3: index out of range" };

	return (&a)[col][row];
}

void adg::Matrix3x3::operator*=(Scalar k) {
	a *= k;
	b *= k;
	c *= k;
}

// -------------------------------------------------------------------------------
// Matrix op (members)

adg::Vector3 adg::Matrix3x3::row(int index) const
{
	if (index < 0 || index > 2)
		throw std::out_of_range{ "Matrix3x3: index out of range" };

	return Vector3{ (*this)(index,0), (*this)(index,1), (*this)(index,2) };
}

adg::Vector3 adg::Matrix3x3::col(int index) const
{
	switch (index) {
	case 0: return a;
	case 1: return b;
	case 2: return c;
	default: throw std::out_of_range{ "Matrix3x3: index out of range" };
	}
}

void adg::Matrix3x3::transpose()
{
	std::swap(a.y, b.x);
	std::swap(a.z, c.x);
	std::swap(b.z, c.y);

}

adg::Matrix3x3 adg::Matrix3x3::transposed() const
{
	return Matrix3x3(row(0), row(1), row(2));
}

// -------------------------------------------------------------------------------
// Notation conversions (members)

// assuming proper rotation matrix
adg::Vector3 adg::Matrix3x3::toEulerAngles() const
{
	Scalar roll{};
	Scalar pitch{};
	Scalar yaw{};

	// computing pitch
	Scalar sinPitch = -(*this)(1,2);
	
	// check domain range for floating point domain range
	// cosTheta = std::max(-1.0, std::min(1.0, cosTheta));
	if (sinPitch <= -Scalar(1))
		pitch = -std::numbers::pi_v<Scalar> * Scalar(0.5);
	else if (sinPitch >= Scalar(1))
		pitch = std::numbers::pi_v<Scalar> * Scalar(0.5);
	else
		pitch = asin(sinPitch);

	// Check Gimbal lock
	if (fabs(sinPitch) > Scalar(0.9999999))
	{
		// we are looking straight up or down
		// set roll to zero and evaluate yaw
		// simplyfing the matrix since cosPitch = 0, roll = 0, sinRoll = 0, cosRoll = 1
		roll = Scalar(0);
		yaw  = atan2(-(*this)(2, 0), (*this)(0, 0));
	}
	else
	{
		// compute yaw and roll
		yaw  = atan2((*this)(0, 2), (*this)(2, 2));
		roll = atan2((*this)(1, 0), (*this)(1, 1));

	}

	return Vector3(toDegrees(pitch), toDegrees(yaw), toDegrees(roll));
}

adg::Vector3 adg::Matrix3x3::toAxisAngle() const
{
	// computing the trace
	Scalar tr = (*this)(0, 0) + (*this)(1, 1) + (*this)(2, 2);

	// tr(R)= 1 + 2cosθ.
	Scalar cosTheta = (tr - Scalar(1)) * Scalar(0.5);
	// clamp
	cosTheta = std::max(-Scalar(1), std::min(Scalar(1), cosTheta));
	Scalar theta = acos(cosTheta);

	// if angle zero no rotation
	if (isZero(theta))
		return Vector3();

	// if angle near pi
	// unstable for sin(pi)
	if (areEqual(theta, std::numbers::pi_v<Scalar>))
	{
		Scalar ax = sqrt(((*this)(0, 0) + 1) * Scalar(0.5));
		Scalar ay = sqrt(((*this)(1, 1) + 1) * Scalar(0.5));
		Scalar az = sqrt(((*this)(2, 2) + 1) * Scalar(0.5));

		// adjust signs
		if ((*this)(0, 1) + (*this)(1, 0) < Scalar(0)) ay = -ay;
		if ((*this)(0, 2) + (*this)(2, 0) < Scalar(0)) az = -az;

		return { Vector3(ax, ay, az).normalized() * theta };
	}

	// general case
	Scalar d = Scalar(1) / Scalar(2) * sin(theta);
	Vector3 axis(((*this)(2, 1) - (*this)(1, 2)) * d,
		         ((*this)(0, 2) - (*this)(2, 0)) * d,
		         ((*this)(1, 0) - (*this)(0, 1)) * d);


	return { axis.normalized() * theta };
}

// -------------------------------------------------------------------------------
// Matrix op (non-members)

// assuming columns vectors
adg::Vector3 adg::applyMatrix3x3(const Matrix3x3 mat, const Vector3& v)
{
	return mat.a * v.x + mat.b * v.y + mat.c * v.z;
}

// assuming row vectors
adg::Vector3 adg::applyMatrix3x3RowRep(const Vector3& v, Matrix3x3& mat)
{
	return Vector3(mat.row(0) * v.x + mat.row(1)*v.y + mat.row(2)*v.z);
}

adg::Matrix3x3 adg::multiplyMatrices(const Matrix3x3& m1, const Matrix3x3& m2)
{
	/*
	Vector3 newColumn1 = m1.a * m2(0,0) + m1.b * m2(1,0) + m1.c * m2(2,0);
	Vector3 newColumn2 = m1.a * m2(0,1) + m1.b * m2(1,1) + m1.c * m2(2,1);
	Vector3 newColumn3 = m1.a * m2(0,2) + m1.b * m2(1,2) + m1.c * m2(2,2);

	return Matrix3x3(newColumn1, newColumn2, newColumn3);
	*/
	
	Vector3 col0 = cleanVector3(applyMatrix3x3(m1, m2.a));
	Vector3 col1 = cleanVector3(applyMatrix3x3(m1, m2.b));
	Vector3 col2 = cleanVector3(applyMatrix3x3(m1, m2.c));
	return Matrix3x3(col0, col1, col2);
}

// -------------------------------------------------------------------------------
// Notation conversions (non-members)

adg::Matrix3x3 adg::axisAngleToRotationMatrix(const Vector3& axis, Scalar angleDeg)
{
	Scalar angleRad = toRadians(angleDeg);

	/*
	Alternative:

	Vector3 nx = rotateVectorAroundAxis(Vector3::right(), axis, angleDeg);
	Vector3 ny = rotateVectorAroundAxis(Vector3::up(), axis, angleDeg);
	Vector3 nz = rotateVectorAroundAxis(Vector3::forward(), axis, angleDeg);

	Matrix3x3 mrot(nx, ny, nz);
	*/

	// as column vectors
	Scalar px = (axis.x * axis.x) * (Scalar(1) - cos(angleRad)) + cos(angleRad);
	Scalar py = (axis.x * axis.y) * (Scalar(1) - cos(angleRad)) + axis.z * sin(angleRad);
	Scalar pz = (axis.x * axis.z) * (Scalar(1) - cos(angleRad)) - axis.y * sin(angleRad);

	Scalar qx = (axis.x * axis.y) * (Scalar(1) - cos(angleRad)) - axis.z * sin(angleRad);
	Scalar qy = (axis.y * axis.y) * (Scalar(1) - cos(angleRad)) + cos(angleRad);
	Scalar qz = (axis.y * axis.z) * (Scalar(1) - cos(angleRad)) + axis.x * sin(angleRad);

	Scalar rx = (axis.x * axis.z) * (Scalar(1) - cos(angleRad)) + axis.y * sin(angleRad);
	Scalar ry = (axis.y * axis.z) * (Scalar(1) - cos(angleRad)) - axis.x * sin(angleRad);
	Scalar rz = (axis.z * axis.z) * (Scalar(1) - cos(angleRad)) + cos(angleRad);

	return Matrix3x3(Vector3(px, py, pz),
		             Vector3(qx, qy, qz),
		             Vector3(rx, ry, rz));
}

adg::Matrix3x3 adg::eulerToRotationMatrix(Scalar in_x, Scalar in_y, Scalar in_z)
{
	Matrix3x3 mroll  = axisAngleToRotationMatrix(Vector3::forward(), in_z);     
	Matrix3x3 mpitch = axisAngleToRotationMatrix(Vector3::right(), in_x);       
	Matrix3x3 myaw   = axisAngleToRotationMatrix(Vector3::up(), in_y);        

	// Z (roll) -> X (pitch) -> Y (yaw)
	// for column vectors
	Matrix3x3 result = multiplyMatrices(myaw, mpitch);
	result = multiplyMatrices(result, mroll);

	return result;
}

// -------------------------------------------------------------------------------
// Utils

adg::Matrix3x3 adg::cleanMatrix3x3(const Matrix3x3& m)
{
	return Matrix3x3(cleanVector3(m.a), cleanVector3(m.b), cleanVector3(m.c));
}

std::ostream& adg::operator<<(std::ostream& os, const Matrix3x3& m)
{
	for (int i = 0; i < 3; ++i)
	{
		Vector3 r = m.row(i);
		os << "| " << r.x << " " << r.y << " " << r.z << " |" << std::endl;
	}

	return os;
}
