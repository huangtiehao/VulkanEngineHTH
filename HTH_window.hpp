#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<string>
class HTH_window {
public:
	HTH_window(int w, int h, std::string name);
	~HTH_window();
	bool shouldClose() { return glfwWindowShouldClose(window); }
private:
	GLFWwindow* window;
	const int width;
	const int height;
	std::string windowName;
	void initWindow();
};