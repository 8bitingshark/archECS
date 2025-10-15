// ----------------------------------------------------------------
// Transform implementation
// ----------------------------------------------------------------

#ifndef ADG_TRANSFORM_H
#define ADG_TRANSFORM_H

#include "Scalar.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace adg {

	class Transform {

	public:
		Vector3 translation{};
		Quaternion rotation{};
		Scalar scale{};

		Transform();
		Transform(const Vector3& position, const Quaternion& rotation, const Scalar& scale);

		// Apply_to
		Vector3 apply_to_point(const Vector3& point) const;
		void applied_to_point(Vector3& point) const;

		Vector3 apply_to_vector(const Vector3& vector) const;
		void applied_to_vector(Vector3& vector) const;

		Vector3 apply_to_versor(const Vector3& versor) const;
		void applied_to_versor(Vector3& versor) const;

		// Transform operations
		void cumulate_with(const Transform& otherT);
		Transform cumulated_with(const Transform& otherT) const;

		void inverse();
		Transform inversed() const;

		void mix(const Transform& otherT, Scalar f);
		Transform mixed(const Transform& otherT, Scalar f) const;

		void print() const;

	};
}

#endif // !ADG_TRANSFORM_H