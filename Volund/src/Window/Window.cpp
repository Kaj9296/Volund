#pragma once

#include "PCH/PCH.h"
#include "Window.h"

#include <glfw/include/GLFW/glfw3.h>

#define GLFW_ERROR_CHECK()\
{\
	const char* Description;\
	int Code = glfwGetError(&Description);\
	if(Description)\
	{\
		VOLUND_ERROR(Description);\
	}\
}\

namespace Volund
{
	static bool GLFWInitialized = false;
	
	void WindowCloseCallback(GLFWwindow* WindowHandle)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);

		Data->Dispatcher->Dispatch(WindowCloseEvent());
	}

	void WindowSizeCallback(GLFWwindow* WindowHandle, int32_t Width, int32_t Height)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);
		
		if (Data->Width != Width || Data->Height != Height)
		{
			Data->Width = Width;
			Data->Height = Height;

			Data->Dispatcher->Dispatch(WindowSizeEvent(Width, Height));
		}
	}

	void KeyCallback(GLFWwindow* WindowHandle, int32_t Key, int32_t Scancode, int32_t Action, int32_t Mods)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);

		Data->Dispatcher->Dispatch(KeyEvent(Key, (bool)Action));
	}

	void MouseButtonCallback(GLFWwindow* WindowHandle, int32_t Button, int32_t Action, int32_t Mods)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);

		Data->Dispatcher->Dispatch(MouseButtonEvent(Button, (bool)Action));
	}

	void ScrollCallback(GLFWwindow* WindowHandle, double xOffset, double yOffset)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);

		Data->Dispatcher->Dispatch(ScrollEvent(xOffset, yOffset));
	}

	void CursorPosCallback(GLFWwindow* WindowHandle, double xPos, double yPos)
	{
		WindowData* Data = (WindowData*)glfwGetWindowUserPointer(WindowHandle);

		Data->Dispatcher->Dispatch(CursorPosEvent(xPos, yPos));
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Window::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(this->_WindowHandle);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(this->_WindowHandle);
	}

	void Window::MakeFullscreen()
	{
		int32_t Count;
		GLFWmonitor** Monitors = glfwGetMonitors(&Count);

		JSON ConfigFile = JSON::Load(VOLUND_CONFIG_JSON);

		int32_t SelectedMonitor = ConfigFile["Window"]["Monitor"];
		if (SelectedMonitor >= Count)
		{
			SelectedMonitor = 0;
		}

		const GLFWvidmode* Mode = glfwGetVideoMode(Monitors[SelectedMonitor]);

		this->_Data.Width = Mode->width;
		this->_Data.Height = Mode->height;

		glfwWindowHint(GLFW_RED_BITS, Mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, Mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, Mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, Mode->refreshRate); 
		
		glfwSetWindowMonitor(this->_WindowHandle, Monitors[SelectedMonitor], 0, 0, Mode->width, Mode->height, Mode->refreshRate);
	}

	void Window::SetVSync(bool Enabled)
	{
		glfwSwapInterval(Enabled);
	}

	Vec2 Window::GetSize()
	{
		return Vec2(this->_Data.Width, this->_Data.Height);
	}

	GLFWwindow* Window::GetWindowHandle()
	{
		return this->_WindowHandle;
	}

	void* Window::GetProcAddress(const char* Name)
	{
		return glfwGetProcAddress(Name);
	}

	Window::Window(Ref<EventDispatcher>& Dispatcher)
	{
		VOLUND_INFO("Creating window...");

		JSON ConfigFile = JSON::Load(VOLUND_CONFIG_JSON);

		this->_Data.Dispatcher = Dispatcher;
		this->_Data.Width = ConfigFile["Window"]["Width"].GetAs<uint32_t>();
		this->_Data.Height = ConfigFile["Window"]["Height"].GetAs<uint32_t>();

		if (!GLFWInitialized)
		{
			glfwInit();
			GLFW_ERROR_CHECK();
			GLFWInitialized = true;
		}

		this->_WindowHandle = glfwCreateWindow(this->_Data.Width, this->_Data.Height, ConfigFile["Window"]["Title"].GetAs<std::string>().c_str(), NULL, NULL);
		GLFW_ERROR_CHECK();

		glfwMakeContextCurrent(_WindowHandle);
		GLFW_ERROR_CHECK();

		//Read settings

		if (ConfigFile["Window"]["FullScreen"] == true)
		{
			this->MakeFullscreen();
		}
		this->SetVSync(ConfigFile["Window"]["VSync"]);

		//Set callbacks
		glfwSetWindowUserPointer(this->_WindowHandle, &this->_Data);
		glfwSetWindowCloseCallback(this->_WindowHandle, WindowCloseCallback);
		glfwSetWindowSizeCallback(this->_WindowHandle, WindowSizeCallback);
		glfwSetKeyCallback(this->_WindowHandle, KeyCallback);
		glfwSetMouseButtonCallback(this->_WindowHandle, MouseButtonCallback);
		glfwSetScrollCallback(this->_WindowHandle, ScrollCallback);
		glfwSetCursorPosCallback(this->_WindowHandle, CursorPosCallback);

		GLFW_ERROR_CHECK();

		VOLUND_INFO("GLFW Version: %s", glfwGetVersionString());
	}

	Window::~Window()
	{
		glfwDestroyWindow(_WindowHandle);

		glfwTerminate();
	
		GLFWInitialized = false;
	}

} //namespace Volund