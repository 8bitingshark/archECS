// ----------------------------------------------------------------
// AffineTransform implementation
// ----------------------------------------------------------------

#include "AffineTransform.h"
#include "Quaternion.h"
#include <iostream>

adg::AffineTransform::AffineTransform() : m(Matrix3x3::identity()), t(Vector3()) {}

adg::AffineTransform::AffineTransform(Vector3 in_t, Quaternion in_q, Scalar in_s)
{
	t = in_t;
	m = cleanMatrix3x3(quaternionToMatrix3x3(in_q));
	m *= in_s;

	/*
	m.a = applyRotationOptimized(Vector3(in_s, 0.0f, 0.0f), in_q);
	m.b = applyRotationOptimized(Vector3(0.0f, in_s, 0.0f), in_q);
	m.c = applyRotationOptimized(Vector3(0.0f, 0.0f, in_s), in_q);
	*/
	
}

// -------------------------------------------------------------------------------
// Apply_to

adg::Vector3 adg::AffineTransform::apply_to_point(const Vector3& point) const
{
	return applyMatrix3x3(m, point) + t;
}

adg::Vector3 adg::AffineTransform::apply_to_vector(const Vector3& vector) const
{
	return applyMatrix3x3(m, vector);
}

adg::Vector3 adg::AffineTransform::apply_to_versor(const Vector3& versor) const
{
	Vector3 result = applyMatrix3x3(m, versor);
	return result.normalized();
}

// -------------------------------------------------------------------------------
// Export

void adg::exportAsRowMatrix4x4(const AffineTransform& at, float* out_matrix)
{
	Vector3 row{};
	for (int i = 0; i < 3; ++i)
	{
		row = at.m.row(i);
		for (int j = 0; j < 3; ++j)
		{
			out_matrix[i * 4 + j] = row[j];
		}
		out_matrix[i * 4 + 3] = at.t[i];
	}

	out_matrix[12] = 0.0f;
	out_matrix[13] = 0.0f;
	out_matrix[14] = 0.0f;
	out_matrix[15] = 1.0f;
}

void adg::exportAsColumnMatrix4x4(const AffineTransform& at, float* out_matrix)
{
	Vector3 col{};
	for (int i = 0; i < 3; ++i)
	{
		col = at.m.col(i);
		for (int j = 0; j < 3; ++j)
		{
			out_matrix[i * 4 + j] = col[j];
		}
		out_matrix[i * 4 + 3] = 0.0f;
	}

	out_matrix[12] = at.t.x;
	out_matrix[13] = at.t.y;
	out_matrix[14] = at.t.z;
	out_matrix[15] = 1.0f;
}

// -------------------------------------------------------------------------------
// Utils

void adg::AffineTransform::print() const
{
	std::cout
		<< "AffineTransform:\n"
		<< "matrix:\n" << m
		<< "translation: " << t;
}

void adg::printMatrix4x4(const float* const matrix)
{
	for (int i = 0; i < 16; ++i)
	{
		std::cout << matrix[i] << " ";
		if (i % 4 == 3)
			std::cout << "\n";
	}
}
