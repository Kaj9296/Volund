#pragma once

#include "Core/Window/Window.h"

namespace Volund
{
	struct LuaWindow
	{
	public:

		void SetCursorMode(CursorMode NewMode);

		void SetTitle(std::string Title);

		void SetVsync(bool Enabled);

	private:

	};
}