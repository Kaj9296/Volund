#pragma once

#include "../EditorWindow.h"

class ViewportWindow : public EditorWindow
{
public:

	void OnProcedure(const VL::Event& e) override;

	ViewportWindow(VL::Ref<EditorContext> context);

private:
	
	class ViewportCamera
	{
	public:

		float LookSpeed = 1.0f;
		float MoveSpeed = 5.0f;
		float ZoomSpeed = 0.1f;

		float FOV = 80.0f;

		VL::Ref<VL::Framebuffer> Framebuffer;

		void Update(VL::Input& input, float timeStep);

		void SubmitToRenderer(ImVec2 viewportSize);

		ViewportCamera();

	private:

		VL::Vec3 m_Position;
		VL::Vec3 m_Rotation;
		VL::Vec3 m_BallCenter;
		VL::Vec3 m_BallRotation;
		float m_Distance;

		VL::IVec2 m_OldMousePosition;
		float m_OldScrollPosition;
	} m_Camera;

	VL::Ref<VL::ImGuiImage> m_ViewportImage;

	VL::Input m_Input;
};
