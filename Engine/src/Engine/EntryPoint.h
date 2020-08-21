#pragma once
#include "Application.h"

#ifdef ENG_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();
	ENG_CORE_FATAL("Testing Fatal");
	ENG_CORE_WARN("Initialized Log!");
	int a = 5;
	ENG_INFO("Hello Var={0}", a);

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}
#endif