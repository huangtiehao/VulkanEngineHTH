#include "HTH_window.hpp"
#include <stdexcept>

HTH_window::HTH_window(int w, int h, std::string name) :width(w), height(h), windowName(name)
{
	initWindow();
}

HTH_window::~HTH_window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void HTH_window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void HTH_window::initWindow()
{
	glfwInit();
	//glfw默认会为opengl设置状态，这里使用GLFW_NO_API
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//不能改变窗口大小
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
