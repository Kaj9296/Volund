 #include "PCH/PCH.h"

#include "UILayer.h"

#include "VML/VML.h"

#include "FileDialog/FileDialog.h"

#include "Editor/EditorLayer/EditorLayer.h"

#include "Widget/EntitiesWidget/EntitiesWidget.h"
#include "Widget/ViewportWidget/ViewportWidget.h"
#include "Widget/InspectorWidget/InspectorWidget.h"

#include <windows.h>

#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_win32.h>

#include "ImGuiStyle.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void UILayer::OnAttach()
{
	static std::string IniFilename = std::filesystem::current_path().string() + "\\imgui.ini";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.Fonts->AddFontFromFileTTF("Data/Fonts/OpenSans-Regular.ttf", 18.0f);
	io.IniFilename = IniFilename.c_str();

	SetupImGuiStyle();
	//ImGui::StyleColorsDark();

	auto EditorWindow = GetLayer<EditorLayer>()->GetWindow();

	EditorWindow->SetProcedureCatch((Volund::ProcCatch)ImGui_ImplWin32_WndProcHandler);

	ImGui_ImplWin32_Init(EditorWindow->GetHandle());
	ImGui_ImplOpenGL3_Init();

	this->_WidgetContainer.PushBack(new EntitiesWidget(this, true));
	this->_WidgetContainer.PushBack(new ViewportWidget(this, true));
	this->_WidgetContainer.PushBack(new InspectorWidget(this, true));
}

void UILayer::OnDetach()
{
	ImGui::SaveIniSettingsToDisk(NULL);
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void UILayer::OnUpdate(TimeStep TS)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	this->BeginDockSpace();
	{
		this->DrawMenuBar();

		for (const auto& View : this->_WidgetContainer)
		{
			for (const auto& Widget : View)
			{
				if (Widget->_IsActive)
				{
					Widget->OnUpdate(TS);
				}
			}
		}
	}	
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UILayer::OnEvent(Event* E)
{
	this->_Input.HandleEvent(E);

	this->HandleShortcuts();

	for (const auto& View : this->_WidgetContainer)
	{
		for (const auto& Widget : View)
		{
			if (Widget->_IsActive)
			{
				Widget->OnEvent(E);
			}
		}
	}
}

void UILayer::HandleShortcuts()
{
	if (this->_Input.IsHeld(VOLUND_KEY_CONTROL))
	{
		auto Editor = GetLayer<EditorLayer>();

		if (this->_Input.IsHeld(VOLUND_KEY_SHIFT))
		{
			if (this->_Input.IsPressed('N'))
			{
				//auto Editor = GetLayer<EditorLayer>();
				//Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
			}

			if (this->_Input.IsPressed('O'))
			{
				Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
			}

			if (Editor->GetProject() != nullptr)
			{
				if (this->_Input.IsPressed('S'))
				{
					Editor->GetProject()->Save(Editor->GetProject()->GetVMLFilepath());
				}

				if (this->_Input.IsPressed('A'))
				{
					Editor->GetProject()->Save(FileDialog::SaveFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
				}
			}
		}
		else if (Editor->GetProject() != nullptr)
		{
			if (this->_Input.IsPressed('N'))
			{
				//auto Editor = GetLayer<EditorLayer>();
				//Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
			}

			if (this->_Input.IsPressed('O'))
			{
				Editor->GetProject()->LoadScene(FileDialog::OpenFile("Volund Scene (*.vscene)\0*.vscene\0", Editor->GetWindow()));
			}

			if (this->_Input.IsPressed('S'))
			{
				Editor->GetProject()->SaveScene(Editor->GetProject()->GetSceneFilepath());
			}

			if (this->_Input.IsPressed('A'))
			{
				Editor->GetProject()->SaveScene(FileDialog::OpenFile("Volund Scene (*.vscene)\0*.vscene\0", Editor->GetWindow()));
			}
		}
	}
}

void UILayer::BeginDockSpace()
{
	static bool Open = true;

	static ImGuiDockNodeFlags DockspaceFlags;
	static ImGuiWindowFlags WindowFlags =
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	const ImGuiViewport* ViewPort = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ViewPort->WorkPos);
	ImGui::SetNextWindowSize(ViewPort->WorkSize);
	ImGui::SetNextWindowViewport(ViewPort->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace Demo", &Open, WindowFlags);

	ImGui::PopStyleVar(3);

	ImGuiID DockspaceID = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(DockspaceID, ImVec2(0.0f, 0.0f), DockspaceFlags);
}

void UILayer::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			this->DrawProjectMenu();

			this->DrawSceneMenu();

			ImGui::EndMenu();
		}

		auto Project = GetLayer<EditorLayer>()->GetProject();

		if (ImGui::BeginMenu("Widgets"))
		{
			for (const auto& View : this->_WidgetContainer)
			{
				for (const auto& Widget : View)
				{
					if (ImGui::MenuItem(Widget->GetName()))
					{
						Widget->_IsActive = true;
					}
				}
			}

			ImGui::EndMenu();
		}

		if (Project != nullptr)
		{
			std::string ProjectName = std::filesystem::path(Project->GetVMLFilepath()).filename().string();
			std::string SceneName = std::filesystem::path(Project->GetSceneFilepath()).filename().string();
			ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ProjectName.c_str()).x - ImGui::CalcTextSize(SceneName.c_str()).x - 50);
			ImGui::Text("%s | %s", ProjectName.c_str(), SceneName.c_str());
		}

		ImGui::EndMenuBar();
	}
}

void UILayer::DrawProjectMenu()
{
	auto Editor = GetLayer<EditorLayer>();

	if (ImGui::BeginMenu("Project"))
	{
		if (ImGui::MenuItem("New", "Ctrl+Shift+N"))
		{
			//auto Editor = GetLayer<EditorLayer>();
			//Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
		}

		if (ImGui::MenuItem("Open", "Ctrl+Shift+O"))
		{
			Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
		}

		if (Editor->GetProject() != nullptr)
		{
			if (ImGui::MenuItem("Save", "Ctrl+Shift+S"))
			{
				Editor->GetProject()->Save(Editor->GetProject()->GetVMLFilepath());
			}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+A"))
			{
				Editor->GetProject()->Save(FileDialog::SaveFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
			}

		}

		ImGui::EndMenu();
	}
}

void UILayer::DrawSceneMenu()
{
	auto Editor = GetLayer<EditorLayer>();

	if (Editor->GetProject() != nullptr)
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				//auto Editor = GetLayer<EditorLayer>();
				//Editor->LoadProject(FileDialog::OpenFile("Volund Project (*.vproj)\0*.vproj\0", Editor->GetWindow()));
			}

			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				Editor->GetProject()->LoadScene(FileDialog::OpenFile("Volund Scene (*.vscene)\0*.vscene\0", Editor->GetWindow()));
			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				Editor->GetProject()->SaveScene(Editor->GetProject()->GetSceneFilepath());
			}

			if (ImGui::MenuItem("Save As", "Ctrl+A"))
			{
				Editor->GetProject()->SaveScene(FileDialog::OpenFile("Volund Scene (*.vscene)\0*.vscene\0", Editor->GetWindow()));
			}

			ImGui::EndMenu();
		}
	}
}