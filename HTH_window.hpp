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
	bool wasWindowResized() { return frameBufferResized; }
	void resetWindowResizedFlag() { frameBufferResized = false; }
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	VkExtent2D getExtent() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
private:
	static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
	GLFWwindow* window;
	int width;
	int height;
	bool frameBufferResized=false;
	std::string windowName;
	void initWindow();
};