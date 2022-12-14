#include "PCH/PCH.h"

#include "Editor.h"

#include "Widget/OutputWidget/OutputWidget.h"
#include "Widget/ViewportWidget/ViewportWidget.h"

void Editor::OnRun()
{
	VL::RenderingAPI::Select(VL::GraphicsAPI::OPENGL);

	this->AttachModule(new VL::WindowModule());

	VL::RenderingAPI::Init();
	VL::Renderer::Init(new VL::ForwardRenderer());

	this->AttachModule(new VL::ImGuiModule());
	this->AttachModule(new VL::LuaModule());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("data/fonts/OpenSans-Regular.ttf", 18.0f);

	this->_Widgets.push_back(VL::Ref<OutputWidget>(new OutputWidget(this)));
	this->_Widgets.push_back(VL::Ref<ViewportWidget>(new ViewportWidget(this)));
}

void Editor::OnTerminate()
{

}

void Editor::Procedure(const VL::Event& E)
{
	VOLUND_PROFILE_FUNCTION();

	this->_Input.HandleEvent(E);

	switch (E.Type)
	{
	case VL::EventType::RENDER:
	{
		this->GetModule<VL::WindowModule>()->GetWindow()->SetVsync(true);

		VL::ImGuiModule::BeginFrame();

		if (VL::ImGuiModule::BeginDockSpace())
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Widget"))
				{
					for (auto& Widget : this->_Widgets)
					{
						if (ImGui::MenuItem(Widget->GetName()))
						{
							Widget->IsActive = true;
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			for (auto& Widget : this->_Widgets)
			{
				if (Widget->IsActive)
				{
					Widget->OnRender();
				}
			}
		}

		ImGui::End();

		VL::ImGuiModule::EndFrame();
	}
	break;	
	case VL::EventType::UPDATE:
	{			
		float TS = VOLUND_EVENT_UPDATE_GET_TIMESTEP(E);

		for (auto& Widget : this->_Widgets)
		{
			if (Widget->IsActive)
			{
				Widget->OnUpdate(TS);
			}
		}
	}
	break;
	case VL::EventType::KEY:
	{
		for (auto& Widget : this->_Widgets)
		{
			if (Widget->IsActive)
			{
				Widget->OnKey(E);
			}
		}
	}
	break;
	case VL::EventType::WINDOW_CLOSE:
	{
		this->Terminate();
	}
	break;
	default:
	{

	}
	break;
	}
}