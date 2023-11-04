#include "firstApp.hpp"

void firstApp::run()
{
	while (!hth_window.shouldClose())
	{
		glfwPollEvents();
	}
}
