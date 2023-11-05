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
	//glfwĬ�ϻ�Ϊopengl����״̬������ʹ��GLFW_NO_API
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//���ܸı䴰�ڴ�С
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
