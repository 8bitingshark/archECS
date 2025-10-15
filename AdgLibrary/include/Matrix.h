// ----------------------------------------------------------------
// Matrix3x3 implementation for the Advanced Graphics
// course of the Master Game Dev
// ----------------------------------------------------------------

#ifndef ADG_MATRIX_H
#define ADG_MATRIX_H
#include "Vector3.h"

namespace adg {

	class Matrix3x3 {

	public:
		// columns vectors
		Vector3 a{};
		Vector3 b{};
		Vector3 c{};

		// constructors
		Matrix3x3();
		Matrix3x3(Vector3 in_a, Vector3 in_b, Vector3 in_c);

		// r-value
		Scalar operator()(int row, int col) const;
		// l-value
		Scalar& operator()(int row, int col);

		void operator*=(Scalar k);

		Vector3 row(int index) const;
		Vector3 col(int index) const;

		void transpose();
		Matrix3x3 transposed() const;
		
		// notation conversions
		Vector3 toEulerAngles() const;
		Vector3 toAxisAngle() const;

		static Matrix3x3 identity() { return Matrix3x3(Vector3(Scalar(1), Scalar(0), Scalar(0)),
			                                           Vector3(Scalar(0), Scalar(1), Scalar(0)),
			                                           Vector3(Scalar(0), Scalar(0), Scalar(1))); }
	};

	Vector3   applyMatrix3x3(const Matrix3x3 mat, const Vector3& v);
	Vector3   applyMatrix3x3RowRep(const Vector3& v, Matrix3x3& mat);
	Matrix3x3 multiplyMatrices(const Matrix3x3& m1, const Matrix3x3& m2);

	// notation conversions
	Matrix3x3 axisAngleToRotationMatrix(const Vector3& axis, Scalar angleDeg);
	Matrix3x3 eulerToRotationMatrix(Scalar in_x, Scalar in_y, Scalar in_z);

	// Utils
	Matrix3x3 cleanMatrix3x3(const Matrix3x3& m);
	std::ostream& operator<<(std::ostream& os, const Matrix3x3& m);
	inline bool areEqual(const Matrix3x3& m1, const Matrix3x3& m2) {
		return areEqual(m1.a, m2.a) && areEqual(m1.b, m2.b) && areEqual(m1.c, m2.c);
	}
}

#endif