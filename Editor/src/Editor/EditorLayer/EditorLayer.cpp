#include "PCH/PCH.h"

#include "EditorLayer.h"

#define LOAD_SCENE

void EditorLayer::LoadScene(Ref<Scene> NewScene)
{
	this->_LoadedScene = NewScene;
}

void EditorLayer::OnAttach()
{
	auto NewEventDispatcher = Ref<EventDispatcher>(new EventDispatcher(this));
	this->_Window = Ref<Window>(new Window(NewEventDispatcher, 500, 500, false));

	this->_Window->SetTitle("Editor");
	this->_Window->SetCursorMode("Disabled");
	this->_Window->SetFocus();

	this->_Context = Context::Create(this->_Window);
	this->_Context->SetVSync(false);
	this->_Context->MakeCurrent();

	Renderer::SetAPI(RenderingAPI::Create());

#ifndef LOAD_SCENE

	Ref<Scene> NewScene = Ref<Scene>(new Scene());

	auto TestShader = NewScene->Assets.Create<ShaderAsset>("Standard/Shaders/Test.shader");
	auto TeapotMesh = NewScene->Assets.Create<MeshAsset>("Standard/Models/Teapot.obj");
	auto CubeMesh = NewScene->Assets.Create<MeshAsset>("Standard/Models/Cube.obj");

	auto TestMaterial = NewScene->Assets.Create<MaterialAsset>("Test.material");

	Ref<Entity> CameraEntity = NewScene->CreateEntity("CameraEntity", Vec3(0.0f, 2.0f, 10.0f));
	CameraEntity->CreateComponent<Camera>()->SetActive();
	CameraEntity->CreateComponent<CameraMovement>(5.0f, 0.5f);

	Ref<Entity> TeapotEnity = NewScene->CreateEntity("TeapotEntity", Vec3(3.0f, 0.0f, 0.0f));
	TeapotEnity->CreateComponent<MeshRenderer>(CubeMesh, TestMaterial);

	Ref<Entity> CubeEnity = NewScene->CreateEntity("CubeEntity", Vec3(-3.0f, 0.0f, 0.0f));
	CubeEnity->CreateComponent<MeshRenderer>(TeapotMesh, TestMaterial);

	Ref<Entity> RedPointLight = NewScene->CreateEntity("RedPointLight", Vec3(10.0f, 5.0f, 0.0f));
	RedPointLight->CreateComponent<PointLight>(RGB(1.0f, 0.0f, 0.0f));

	Ref<Entity> BluePointLight = NewScene->CreateEntity("BluePointLight", Vec3(-10.0f, 5.0f, 0.0f));
	BluePointLight->CreateComponent<PointLight>(RGB(0.0f, 0.0f, 1.0f));

	NewScene->Serialize("Test.scene");

#else

	Ref<Scene> NewScene = Scene::Deserialize("Test.scene");

#endif

	this->LoadScene(NewScene);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(TimeStep TS)
{
	this->_Context->MakeCurrent();

	Camera* ActiveCamera = Camera::GetActiveCamera(this->_LoadedScene);

	if (ActiveCamera != nullptr)
	{
		Mat4x4 ViewProjMatrix = ActiveCamera->GetProjectionMatrix(this->_Context->GetWindow()->GetAspectRatio()) * ActiveCamera->GetViewMatrix();
		Vec3 EyePosition = ActiveCamera->GetEntity()->GetComponent<Transform>()->Position;

		auto const& PointLightView = this->_LoadedScene->ComponentView<PointLight>();
		std::vector<PointLightData> PointLights;
		PointLights.reserve(PointLightView.size());
		for (auto const& Light : PointLightView)
		{
			PointLights.push_back({Light->Color, Light->GetEntity()->GetComponent<Transform>()->Position});
		}

		Renderer::BeginScene(ViewProjMatrix, EyePosition, PointLights);

		if (_LoadedScene != nullptr)
		{
			this->_LoadedScene->Update(TS);
		}
	
		Renderer::EndScene(this->_Context);
	}
	else
	{
		if (_LoadedScene != nullptr)
		{
			this->_LoadedScene->Update(TS);
		}
	}

	this->_Window->Update();
}

void EditorLayer::OnEvent(Event* E)
{
	switch (E->GetType())
	{
	case EventType::WINDOW_CLOSE:
	{
		this->GetApp()->Terminate();
	}
	break;
	}

	if (_LoadedScene != nullptr)
	{
		this->_LoadedScene->EventCallback(E);
	}
}