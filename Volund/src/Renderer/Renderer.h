#pragma once

#include "Mesh/Mesh.h"
#include "Material/Material.h"
#include "RenderingAPI/RenderingAPI.h"

#include "UniformBuffer/UniformBuffer.h"

#include "Renderer/Framebuffer/Framebuffer.h"

#define VOLUND_UNIFORM_BUFFER_BINDING_CAMERA 0
#define VOLUND_UNIFORM_BUFFER_BINDING_LIGHTS 1
#define VOLUND_MAX_LIGHTS 64

namespace Volund
{
	struct RendererCommand
	{
		Mat4x4 ModelMatrix;
		Ref<Mesh> mesh;
		Ref<Material> material;

		bool Discriminated = false;
	};

	struct RendererLight
	{
		RGB Color;
		Vec3 Position;
		float Brightness;
	};

	struct RendererEye
	{
		Mat4x4 ViewMatrix;
		Mat4x4 ProjectionMatrix;

		bool DrawToScreen = true;
		Ref<Framebuffer> Target;
	};

	class RendererInstance
	{
	public:

		virtual void Begin() = 0;

		virtual void Submit(const RendererCommand& Command) = 0;

		virtual void Submit(const RendererLight& Light) = 0;

		virtual void Submit(const RendererEye& Eye) = 0;

		virtual void End() = 0;

		RendererInstance() = default;

		virtual ~RendererInstance() = default;

	protected:

		struct Data
		{
			void Sort();

			void Discriminate(const RendererEye& Eye);

			std::vector<RendererCommand> CommandQueue;
			std::vector<RendererLight> Lights;
			std::vector<RendererEye> Eyes;
		} _Data;

		Ref<RenderingAPI> _API;

		Ref<UniformBuffer> _CameraUniforms;

		Ref<UniformBuffer> _LightsUniforms;

	private:
	};

	class Renderer
	{
	public:
		
		static void Init(const Ref<RendererInstance>& Instance);

		static void Init(RendererInstance* Instance);

		static void Reset();

		static void Begin();

		static void Submit(const RendererCommand& Command);

		static void Submit(const RendererLight& Light);

		static void Submit(const RendererEye& Eye);

		static void End();

		Renderer() = delete;

	private:

		static inline Ref<RendererInstance> _Instance;
	};
}

