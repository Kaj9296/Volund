#pragma once

#include "../Component.h"

#include "Renderer/Material/Material.h"
#include "Renderer/Mesh/Mesh.h"

namespace Volund
{
	class MeshRenderer : public Component
	{
	public:

		void SetLayer(uint8_t Layer);

		void SetMesh(Ref<Mesh> NewMesh);

		void SetMaterial(Ref<Material> NewMaterial);

		Ref<Mesh> GetMesh();

		Ref<Material> GetMaterial();

		void Procedure(const Event& E);

		MeshRenderer() = default;

		MeshRenderer(Ref<Mesh> MeshRef, Ref<Material> MaterialRef);

	private:

		uint16_t _LayerMask = 1;

		Ref<Mesh> _Mesh;
		Ref<Material> _Material;
	};
}
