#pragma once

#include "Context.h"
#include "Window/Window.h"

namespace Volund
{
	class OpenGLContext : public Context
	{
	public:

		void SwapBuffers() override;

		OpenGLContext(Window* window);

	private:
		
		Window* _Window;
	};
}
