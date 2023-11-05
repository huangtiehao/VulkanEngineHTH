#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<string>
class HTH_window {
public:
	HTH_window(int w, int h, std::string name);
	~HTH_window();
	//½ûÖ¹¿½±´¹¹Ôìº¯Êý
	HTH_window(const HTH_window&) = delete;
	//½ûÖ¹¸³Öµº¯Êý
	HTH_window &operator=(const HTH_window&) = delete;
	bool shouldClose() { return glfwWindowShouldClose(window); }
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
private:
	GLFWwindow* window;
	const int width;
	const int height;
	std::string windowName;
	void initWindow();
};