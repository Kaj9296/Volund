#pragma once

#include "Lua/LuaComponent/LuaComponent.h"
#include "Lua/LuaComponent/LuaComponents.h"

#include "Scene/Component/MeshRenderer/MeshRenderer.h"

#include "Lua/LuaMesh/LuaMesh.h"
#include "Lua/LuaMaterial/LuaMaterial.h"

namespace Volund
{
	struct LuaMeshRenderer
	{
	public:

		void SetMesh(LuaMesh NewMesh);
		void SetMaterial(LuaMaterial NewMaterial);

		LuaMesh GetMesh();
		LuaMaterial GetMaterial();

		LuaMeshRenderer(Ref<MeshRenderer> MeshRenderer);

	private:

		Ref<MeshRenderer> _MeshRenderer;
	};
}