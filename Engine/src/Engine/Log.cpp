#include "epch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	bool Log::s_IsInitialized = false;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);
		
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_IsInitialized = true;
	}

	void Log::Destroy()
	{
		spdlog::drop_all();
		s_CoreLogger = nullptr;
		s_ClientLogger = nullptr;
		s_IsInitialized = false;
	}
}