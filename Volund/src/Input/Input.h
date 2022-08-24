#pragma once

#include "EventDispatcher/Event.h"

#define KEY_AMOUNT GLFW_KEY_LAST
#define MOUSE_BUTTON_AMOUNT GLFW_MOUSE_BUTTON_LAST
 
#define MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_MIDDLE
#define MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_LEFT   
#define MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_RIGHT   

#include "Math/Math.h"

namespace Volund
{
	void KeyHandler(Event* E);
	void MouseButtonHandler(Event* E);
	void ScrollHandler(Event* E);
	void CursorPosHandler(Event* E);

	class Input
	{
	public:

		static bool IsHeld(char KeyCode);
		static bool IsPressed(char KeyCode);

		static bool IsMouseButtonHeld(char Button);
		static bool IsMouseButtonPressed(char Button);

		static int32_t GetScrollOffset();

		static Vec2 GetCursorPosition();

	private:
		
		friend void Volund::KeyHandler(Event* E);
		friend void Volund::MouseButtonHandler(Event* E);
		friend void Volund::ScrollHandler(Event* E);
		friend void Volund::CursorPosHandler(Event* E);

		static void SendKeyEvent(KeyEvent* KE);
		static void SendMouseButtonEvent(MouseButtonEvent* MBE);
		static void SendScrollEvent(ScrollEvent* SE);
		static void SendCursorPosEvent(CursorPosEvent* CPE);

		static Vec2 MousePosition;

		static int32_t ScrollOffset;

		static int8_t MouseButtons[MOUSE_BUTTON_AMOUNT];
		static int8_t Keys[KEY_AMOUNT];
	};
}