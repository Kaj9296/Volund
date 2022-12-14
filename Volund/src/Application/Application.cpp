#include "PCH/PCH.h"

#include "Application.h"

#include "Time/Time.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderingAPI/RenderingAPI.h"

#include "ThreadPool/ThreadPool.h"

#include "DelayedTaskHandler/DelayedTaskHandler.h"

namespace Volund
{
	void Application::Run()
	{
		this->OnRun();
		this->Loop();
	}

	bool Application::ShouldRun() const
	{
		return this->_ShouldRun;
	}

	Ref<EventDispatcher> Application::GetEventDispatcher()
	{
		return this->_EventDispatcher;
	}

	void Application::Terminate()
	{
		this->OnTerminate();
		this->_ShouldRun = false;
	}

	void Application::Loop()
	{
		VOLUND_PROFILE_FUNCTION();

		std::chrono::time_point<std::chrono::steady_clock> OldTime = std::chrono::high_resolution_clock::now();

		while (this->_ShouldRun)
		{
			VOLUND_PROFILE_SCOPE("MainLoop");

			std::chrono::duration<double> Duration = std::chrono::high_resolution_clock::now() - OldTime;
			OldTime = std::chrono::high_resolution_clock::now();
			TimeStep TS = TimeStep(Duration.count());

			this->_ThreadPool.Submit([this, TS]()
			{
				Event E = Event(EventType::UPDATE);
				VOLUND_EVENT_UPDATE_SET_TIMESTEP(E, float(TS));
				this->_EventDispatcher->Dispatch(E);
			});

			Renderer::Begin();

			Event E = Event(EventType::RENDER);
			this->_EventDispatcher->Dispatch(E);

			Renderer::End();

			while (this->_ThreadPool.Busy());

			DelayedTaskHandler::Execute();
		}
	}

	Application::Application()
	{
#ifdef VOLUND_DEBUG
		VOLUND_INFO("Initializing application (Debug)...");
#elif VOLUND_RELEASE
		VOLUND_INFO("Initializing application (Release)...");
#elif VOLUND_DIST
		VOLUND_INFO("Initializing application (Distribution)...");
#else
		VOLUND_WARNING("Initializing application (Unknown)...");
#endif	

		this->_EventDispatcher = std::make_shared<EventDispatcher>(this);
	}

	Application::~Application()
	{
		for (const auto& [TypeID, View] : this->_Modules)
		{
			for (const auto& Module : View)
			{
				Module->OnDetach();
			}
		}
	}
}
