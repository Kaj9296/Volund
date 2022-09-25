#include "PCH/PCH.h"
#include "MeshRenderer.h"

#include "Renderer/Renderer.h"

#include "Scene/Component/Transform/Transform.h"

namespace Volund
{
	void MeshRenderer::OnUpdate(TimeStep TS)
	{
		if (this->_Mesh != nullptr && this->_Material != nullptr && this->GetScene()->HasComponent<Transform>(this->GetEntity()))
		{
			Ref<Transform> EntityTransform = this->GetScene()->GetComponent<Transform>(this->GetEntity());
			Mat4x4 ModelMatrix = EntityTransform->GetModelMatrix();

			Renderer::SubmitObject(ModelMatrix, this->_Mesh, this->_Material);
		}
	}

	VML MeshRenderer::Serialize()
	{
		VML MeshRendererVML;

		MeshRendererVML.PushBack("Type", VMLEntry("MeshRenderer"));

		MeshRendererVML.PushBack("Mesh", VMLEntry(this->_Mesh->GetFilepath()));
		MeshRendererVML.PushBack("Material", VMLEntry(this->_Material->GetFilepath()));

		return MeshRendererVML;
	}

	void MeshRenderer::SetMesh(Ref<Mesh> NewMesh)
	{
		this->_Mesh = NewMesh;
	}

	void MeshRenderer::SetMaterial(Ref<Material> NewMaterial)
	{
		this->_Material = NewMaterial;
	}

	const Ref<Mesh>& MeshRenderer::GetMesh()
	{
		return this->_Mesh;
	}

	const Ref<Material>& MeshRenderer::GetMaterial()
	{
		return this->_Material;
	}

	MeshRenderer::MeshRenderer(Ref<Mesh> MeshRef, Ref<Material> MaterialRef)
	{
		this->_Mesh = MeshRef;
		this->_Material = MaterialRef;
	}
}