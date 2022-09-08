#pragma once

#include "PCH/PCH.h"
#include "Logger.h"

namespace Volund
{
	Logger Logger::_CoreLogger = Logger("VOLUND");
	Logger Logger::_ClientLogger = Logger("CLIENT");

	Logger& Logger::GetCoreLogger()
	{
		return _CoreLogger;
	}

	Logger& Logger::GetClientLogger()
	{
		return _ClientLogger;
	}

	void Logger::Info(const char* Format, ...) const
	{
		std::va_list Args;
		va_start(Args, Format);

		printf("\033[0;31m[%s]\033[0;32m %s: ", std::format("{:%H:%M:%OS}", std::chrono::system_clock::now()).c_str(),
		       this->_Name.c_str());
		vprintf(Format, Args);
		printf("\n");

		va_end(Args);
	}

	void Logger::Warning(const char* Format, ...) const
	{
		std::va_list Args;
		va_start(Args, Format);

		printf("\033[0;31m[%s]\033[0;33m %s: ", std::format("{:%H:%M:%OS}", std::chrono::system_clock::now()).c_str(),
		       this->_Name.c_str());
		vprintf(Format, Args);
		printf("\n");

		va_end(Args);
	}

	void Logger::Error(const char* Format, ...) const
	{
		std::va_list Args;
		va_start(Args, Format);

		printf("\033[0;31m[%s] %s: ", std::format("{:%H:%M:%OS}", std::chrono::system_clock::now()).c_str(),
		       this->_Name.c_str());
		vprintf(Format, Args);
		printf("\n");

		va_end(Args);

		abort();
	}

	Logger::Logger(const std::string& Name)
	{
		this->_Name = Name;
	}
} //namespace Volund
