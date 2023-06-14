#include "PCH/PCH.h"
#include "ForwardRenderer.h"

#include "Frustum/Frustum.h"

namespace Volund
{
	void ForwardRenderer::Begin(Ref<Framebuffer> defaultBuffer)
	{
		const uint64_t oldCommandSize = this->m_Data.Models.size();

		this->m_Data = Data();
		this->m_Data.DefaultBuffer = defaultBuffer;
		this->m_Data.Models.reserve(oldCommandSize);
	}

	void ForwardRenderer::End()
	{
		this->m_Data.Sort();

		this->UpdateLightUniforms();

		for (const auto& eye : this->m_Data.Eyes)
		{
			Ref<Framebuffer> targetBuffer;
			if (eye.Target == nullptr)
			{
				targetBuffer = this->m_Data.DefaultBuffer;
			}
			else
			{
				targetBuffer = eye.Target;
			}

			this->m_Data.Discriminate(eye);

			this->UpdateCameraUniforms(eye);

			targetBuffer->Bind();
			auto& targetSpec = targetBuffer->GetSpec();

			VL::RenderingAPI::Clear();						
			VL::RenderingAPI::SetViewPort(0, 0, (int32_t)targetSpec.Width, (int32_t)targetSpec.Height);

			Ref<Material> prevMaterial = nullptr;
			for (const auto& model : this->m_Data.Models)
			{
				if (!model.Discriminated)
				{
					if (model.material != prevMaterial)
					{
						model.material->UpdateShader();
						prevMaterial = model.material;
					}

					Ref<Shader> materialShader = model.material->GetShader();

					if (materialShader != nullptr && materialShader->HasUniform(VOLUND_UNIFORM_NAME_MODELMATRIX))
					{
						materialShader->SetMat4x4(VOLUND_UNIFORM_NAME_MODELMATRIX, model.ModelMatrix);
					}

					model.mesh->Bind();
					VL::RenderingAPI::DrawIndexed(model.mesh);
				}
			}

			targetBuffer->Unbind();
		}
	}
}