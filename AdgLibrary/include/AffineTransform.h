// ----------------------------------------------------------------
// AffineTransform implementation for the Advanced Graphics
// course of the Master Game Dev
// ----------------------------------------------------------------

#ifndef ADG_AFFINE_TRANSFORM_H
#define ADG_AFFINE_TRANSFORM_H

#include "Matrix.h"

namespace adg {

	class Quaternion;

	class AffineTransform {

	public:

		Matrix3x3 m{};    // storing rotation, scaling, shearing
		Vector3   t{};    // translation

		AffineTransform();
		AffineTransform(Vector3 in_t, Quaternion in_q, Scalar in_s);

		Vector3 apply_to_point(const Vector3& point) const;
		Vector3 apply_to_vector(const Vector3& vector) const;
		Vector3 apply_to_versor(const Vector3& versor) const;

		void print() const;
	};

	void exportAsRowMatrix4x4(const AffineTransform& at, float* out_matrix);     // for DirectX C++
	void exportAsColumnMatrix4x4(const AffineTransform& at, float* out_matrix);  // for OpenGL
	void printMatrix4x4(const float* const matrix);
}

#endif //!ADG_AFFINE_TRANSFORM_H