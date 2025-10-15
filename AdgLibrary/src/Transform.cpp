// ----------------------------------------------------------------
// Transform implementation
// ----------------------------------------------------------------

#include "Transform.h"

adg::Transform::Transform(){}

adg::Transform::Transform(const Vector3& position, const Quaternion& rotation, const Scalar& scale)
	: translation(position), rotation(rotation), scale(scale)
{
}

// -------------------------------------------------------------------------------
// Apply_to

adg::Vector3 adg::Transform::apply_to_point(const Vector3& point) const
{
	return applyRotationOptimized(point * scale, rotation) + translation;
}

void adg::Transform::applied_to_point(Vector3& point) const
{
	point = applyRotationOptimized(point * scale, rotation) + translation;
}

adg::Vector3 adg::Transform::apply_to_vector(const Vector3& vector) const
{
	return applyRotationOptimized(vector * scale, rotation);
}

void adg::Transform::applied_to_vector(Vector3& vector) const
{
	vector = applyRotationOptimized(vector * scale, rotation);
}

adg::Vector3 adg::Transform::apply_to_versor(const Vector3& versor) const
{
	return applyRotationOptimized(versor, rotation);
}

void adg::Transform::applied_to_versor(Vector3& versor) const
{
	versor = applyRotationOptimized(versor, rotation);
}

// -------------------------------------------------------------------------------
// Transform operations

void adg::Transform::cumulate_with(const Transform& otherT)
{
	scale = otherT.scale * scale;
	rotation = otherT.rotation * rotation;
	translation = applyRotationOptimized(translation * otherT.scale, otherT.rotation) + otherT.translation;
}

adg::Transform adg::Transform::cumulated_with(const Transform& otherT) const
{
	Transform result{};

	result.scale = otherT.scale * this->scale;
	result.rotation = otherT.rotation * this->rotation; // inverse order, check cumulate quaternion
	result.translation = applyRotationOptimized(this->translation * otherT.scale, otherT.rotation) + otherT.translation;

	return result;
}

void adg::Transform::inverse()
{
	// valid only for uniform scaling

	scale = Scalar(1) / scale;
	rotation = inverseQuaternion(rotation);
	translation = translation * -Scalar(1);
	// take in account the application order
	translation = applyRotationOptimized(translation * scale, rotation);
}

adg::Transform adg::Transform::inversed() const
{
	// valid only for uniform scaling
	Transform result{};

	result.scale = Scalar(1) / this->scale;
	result.rotation = inverseQuaternion(this->rotation);
	result.translation = this->translation * -Scalar(1);
	// take in account the application order
	result.translation = applyRotationOptimized(result.translation * result.scale, result.rotation);

	return result;
}

void adg::Transform::mix(const Transform& otherT, Scalar f)
{
	scale       = lerp (otherT.scale,       scale,       f);
	rotation    = slerp(otherT.rotation,    rotation,    f);
	translation = lerp (otherT.translation, translation, f);
}

adg::Transform adg::Transform::mixed(const Transform& otherT, Scalar f) const
{
	Transform result{};

	result.scale       = lerp (otherT.scale,       this->scale,       f);
	result.rotation    = slerp(otherT.rotation,    this->rotation,    f);
	result.translation = lerp (otherT.translation, this->translation, f);

	return result;
}

// Utils
void adg::Transform::print() const
{
	std::cout
		<< "Transform:\n"
		<< "translation:" << translation << "\n"
		<< "rotation: " << cleanVector3(quaternionToEulerAngles(rotation)) << "\n"
		<< "scale: " << scale;
}
