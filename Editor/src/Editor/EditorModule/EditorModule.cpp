#include "PCH/PCH.h"
#include "EditorModule.h"

#include "Lua/Serializer/SceneSerializer/SceneSerializer.h"

VL::Ref<VL::Scene> EditorModule::GetScene()
{
	if (this->m_GameState != nullptr)
	{
		return this->m_GameState->GetScene();
	}
	else
	{
		return nullptr;
	}
}

std::string EditorModule::GetSceneFilepath()
{
	return this->m_SceneFilepath;
}

void EditorModule::OnAttach(VL::Application* app)
{
	if (!app->HasModule<VL::WindowModule>())
	{
		VOLUND_ERROR("Cant attach a editorModule to an app without a WindowModule!");
	}

	this->m_GameWindow = app->GetModule<VL::WindowModule>()->GetWindow();
}

void EditorModule::OnDetach()
{
}

void EditorModule::Procedure(const VL::Event& e)
{
	if (e.Type == VL::EventType::Update)
	{
		return;
	}

	if (this->m_GameState != nullptr)
	{
		this->m_GameState->Procedure(e);
	}
}

void EditorModule::SaveScene(const std::string& filepath)
{
	if (this->GetScene() == nullptr)
	{
		return;
	}

	std::unique_lock lock(this->m_Mutex);

	VL::DeferredTaskHandler::DeferTask([this, filepath]()
	{
		this->m_SceneFilepath = filepath;

		auto sceneSerializer = VL::SceneSerializer(this->GetScene());

		sceneSerializer.WriteToFile(filepath);
	});
}

void EditorModule::LoadNewScene(const std::string& filepath)
{
	std::unique_lock lock(this->m_Mutex);

	VL::DeferredTaskHandler::DeferTask([this, filepath]()
	{
		this->m_SceneFilepath = filepath;

		this->m_GameState.reset();

		auto luaGameState = VL::LuaGameState(this->m_GameWindow, this->m_SceneFilepath);

		this->m_GameState = luaGameState.Get();
	});
}
