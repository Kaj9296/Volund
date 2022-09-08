#include "PCH/PCH.h"

#include "Transform.h"

namespace Volund
{
	void Transform::SetRotation(const Vec3& Rotation)
	{
		this->Quaternion = Quat(Math::ToRadians(Rotation));
	}

	Vec3 Transform::GetRotation() const
	{
		return Math::ToDegrees(eulerAngles(this->Quaternion));
	}

	void Transform::AddRotation(const Vec3& Rotation)
	{
		this->Quaternion *= Quat(Math::ToRadians(Rotation));
	}

	Vec3 Transform::GetFront() const
	{
		return this->_Front;
	}

	Vec3 Transform::GetRight() const
	{
		return this->_Right;
	}

	Vec3 Transform::GetUp() const
	{
		return this->_Up;
	}

	Mat4x4 Transform::GetModelMatrix() const
	{
		return this->_ModelMatrix;
	}

	void Transform::OnUpdate(TimeStep TS)
	{
		this->_Front = this->Quaternion * Math::Back; //Dont ask
		this->_Right = this->Quaternion * Math::Right;
		this->_Up = this->Quaternion * Math::Up;

		this->_ModelMatrix = Mat4x4(1.0f);

		this->_ModelMatrix = translate(this->_ModelMatrix, this->Position);
		this->_ModelMatrix *= Mat4x4(this->Quaternion);
		this->_ModelMatrix = scale(this->_ModelMatrix, this->Scale);
	}

	JSON Transform::Serialize()
	{
		JSON TransformJSON;

		TransformJSON.AddEntry("Type", "Transform");

		TransformJSON.AddList("Position", {this->Position.x, this->Position.y, this->Position.z});

		Vec3 Rotation = this->GetRotation();
		TransformJSON.AddList("Rotation", {Rotation.x, Rotation.y, Rotation.z});

		TransformJSON.AddList("Scale", {Scale.x, Scale.y, Scale.z});

		return TransformJSON;
	}

	Transform::Transform(const Vec3& Position, const Vec3& Rotation, const Vec3& Scale)
	{
		this->Position = Position;
		this->Scale = Scale;
		this->SetRotation(Rotation);
		this->OnUpdate(0.0f);
	}
}