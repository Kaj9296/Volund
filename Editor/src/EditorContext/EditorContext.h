#pragma once

#define EDITOR_EVENT_TYPE_NEW_SCENE (VOLUND_HIGHEST_EVENT_TYPE + 1)

class EditorContext : public VL::Module
{
public:

	VL::Entity SelectedEntity = 1;

	void OnAttach(VL::Application* app) override;

	void OnDetach() override;

	void Procedure(const VL::Event& e) override;

	bool IsPaused();

	void Play();

	void Pause();

	std::string GetScenePath();

	std::string GetParentPath();

	VL::Ref<VL::AssetManager> GetAssetmanager();

	VL::Ref<VL::Scene> GetScene();

	VL::Ref<VL::Window> GetWindow();

	void LoadScene(const std::string& filepath);

	void SaveScene(const std::string& filepath);

private:		
	
	bool m_Paused = true;

	std::mutex m_Mutex;

	VL::Ref<VL::EventDispatcher> m_Dispatcher;

	VL::Ref<VL::GameState> m_GameState;

	VL::Ref<VL::Window> m_GameWindow;
};

