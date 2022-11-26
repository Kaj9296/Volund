#include "PCH/PCH.h"
#include "MeshRenderer.h"

#include "Scene/Scene.h"
#include "Renderer/Renderer.h"

#include "Scene/Component/Transform/Transform.h"

namespace Volund
{
	void MeshRenderer::OnUpdate(TimeStep TS)
	{
		if (this->_Mesh != nullptr && this->_Material != nullptr)
		{
			RendererCommand Command;
			Command.mesh = this->_Mesh;
			Command.material = this->_Material;

			if (this->GetScene()->HasComponent<Transform>(this->GetEntity()))
			{
				Command.ModelMatrix = this->GetScene()->GetComponent<Transform>(this->GetEntity())->GetModelMatrix();
			}
			else
			{
				Command.ModelMatrix = Mat4x4(1.0f);
			}

			Renderer::Submit(Command);
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
