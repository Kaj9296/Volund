#pragma once

#include "EventDispatcher/EventDispatcher.h"

namespace Volund
{
	struct WindowData
	{
		uint64_t Width = 0;
		uint64_t Height = 0;

		bool FullScreen = false;

		bool CaptureMouse = false;
		bool ShowMouse = true;

		Ref<EventDispatcher> Dispatcher = nullptr;
	};

	class Window
	{
	public:
		void SwapBuffers() const;

		void Update();

		void SetFocus() const;

		void SetCursorMode(const std::string& NewMode);

		void SetTitle(const std::string& Title);

		Vec2 GetSize() const;

		float GetAspectRatio() const;

		void* GetDeviceContext() const;

		Window(Ref<EventDispatcher>& Dispatcher, uint64_t Width, uint64_t Height, bool FullScreen);

		~Window();

	private:
		std::wstring ConvertToWString(const std::string& String);

		void* _Handle = nullptr;

		void* _Instance = nullptr;

		void* _DeviceContext = nullptr;

		WindowData _Data;
	};
} //namespace Volund
