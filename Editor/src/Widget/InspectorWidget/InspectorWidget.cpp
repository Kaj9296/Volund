#include "PCH/PCH.h"

#include "InspectorWidget.h"

#include "Editor/Editor.h"
#include "Editor/EditorModule/EditorModule.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

const char* InspectorWidget::GetName()
{
	return "Inspector";
}

void InspectorWidget::OnRender()
{
	auto scene = this->m_App->GetModule<VL::GameModule>()->GetScene();

	auto selectedEntity = this->m_App->GetModule<EditorModule>()->SelectedEntity;

	if (ImGui::Begin("Inspector", &this->IsActive))
	{
		if (scene != nullptr && scene->IsEntityRegistered(selectedEntity))
		{
			ImGui::Separator();

			this->DrawComponents();

			ImGui::Separator();

			this->DrawAddComponents();
		}
		else
		{
			ImGui::Text("No Entity Selected!");
		}

	}

	ImGui::End();
}

void InspectorWidget::DrawComponents()
{
	auto scene = this->m_App->GetModule<VL::GameModule>()->GetScene();

	auto selectedEntity = this->m_App->GetModule<EditorModule>()->SelectedEntity;

	this->DrawComponent<VL::Tag>("Tag", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto tag = scene->GetComponent<VL::Tag>(selectedEntity, i);

		std::string newTag = this->TextControl("Tag", tag->String);
		if (newTag != "")
		{
			tag->String = newTag;
		}
	});

	this->DrawComponent<VL::Transform>("Transform", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto transform = scene->GetComponent<VL::Transform>(selectedEntity, i);

		VL::Vec3 position = transform->Position;
		VL::Vec3 rotation = transform->GetRotation();
		VL::Vec3 scale = transform->Scale;

		this->Vec3Control("Position", &position, 0.1f, 0.0f);
		this->Vec3Control("Rotation", &rotation, 0.1f, 0.0f);
		this->Vec3Control("Scale", &scale, 0.1f, 1.0f);

		transform->Position = position;
		transform->SetRotation(rotation);
		transform->Scale = scale;
	});

	this->DrawComponent<VL::Camera>("Camera", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto camera = scene->GetComponent<VL::Camera>(selectedEntity, i);

		//TODO: Add targetbuffer

		this->FloatControl("FOV", &camera->FOV);
		this->FloatControl("FarPlane", &camera->FarPlane);
		this->FloatControl("NearPlane", &camera->NearPlane);
	});

	this->DrawComponent<VL::MeshRenderer>("MeshRenderer", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto meshRenderer = scene->GetComponent<VL::MeshRenderer>(selectedEntity, i);
		auto window = this->m_App->GetModule<VL::WindowModule>()->GetWindow();

		std::string defaultMaterial = scene->FetchAssetFilepath<VL::Material>(meshRenderer->GetMaterial());
		auto selectedMaterial = FileSelectorControl("Material", defaultMaterial, window);
		if (selectedMaterial != "")
		{
			meshRenderer->SetMaterial(scene->FetchAsset<VL::Material>(selectedMaterial));
		}

		std::string defaultMesh = scene->FetchAssetFilepath<VL::Mesh>(meshRenderer->GetMesh());
		auto selectedMesh = FileSelectorControl("Mesh", defaultMesh, window);
		if (selectedMesh != "")
		{
			meshRenderer->SetMesh(scene->FetchAsset<VL::Mesh>(selectedMesh));
		}
	});

	this->DrawComponent<VL::PointLight>("PointLight", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto pointLight = scene->GetComponent<VL::PointLight>(selectedEntity, i);

		ImGui::ColorPicker3("##Color", glm::value_ptr(pointLight->Color), ImGuiColorEditFlags_Float);

		FloatControl("Brightness", &pointLight->Brightness);
	});

	this->DrawComponent<VL::CameraMovement>("CameraMovement", selectedEntity, [this, selectedEntity, scene](int i)
	{
		auto cameraMovement = scene->GetComponent<VL::CameraMovement>(selectedEntity, i);

		FloatControl("Speed", &cameraMovement->Speed);
		FloatControl("Sensitivity", &cameraMovement->Sensitivity);
	});
}

void InspectorWidget::DrawAddComponents()
{
	auto scene = this->m_App->GetModule<VL::GameModule>()->GetScene();

	auto selectedEntity = this->m_App->GetModule<EditorModule>()->SelectedEntity;

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

	Align(ImGui::CalcTextSize("Add Component").x, 0.5f);
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("Add Component");
	}

	if (ImGui::BeginPopup("Add Component"))
	{
		if (ImGui::MenuItem("Tag"))
		{
			scene->CreateComponent<VL::Tag>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Transform"))
		{
			scene->CreateComponent<VL::Transform>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Camera"))
		{
			scene->CreateComponent<VL::Camera>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("MeshRenderer"))
		{
			scene->CreateComponent<VL::MeshRenderer>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("PointLight"))
		{
			scene->CreateComponent<VL::PointLight>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("CameraMovement"))
		{
			scene->CreateComponent<VL::CameraMovement>(selectedEntity);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

InspectorWidget::InspectorWidget(VL::Application* app)
{
	this->m_App = app;
}