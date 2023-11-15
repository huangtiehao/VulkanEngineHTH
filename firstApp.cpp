#include "firstApp.hpp"
#include "simple_render_system.hpp"
#include<stdexcept>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

firstApp::firstApp()
{
	loadGameObjects();
}
firstApp::~firstApp()
{
	
}
void firstApp::run()
{
	Simple_render_system simpleRenderSystem{ hth_device, hth_renderer.getSwapChainRenderPass() };
	while (!hth_window.shouldClose())
	{
		glfwPollEvents();
		auto commandBuffer = hth_renderer.beginFrame();
		if (commandBuffer)
		{
			hth_renderer.beginSwapchainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer,gameObjects);
			hth_renderer.endSwapchainRenderPass(commandBuffer);
			hth_renderer.endFrame();
		}
	}
	vkDeviceWaitIdle(hth_device.device());
}



void firstApp::loadGameObjects()
{
	std::vector<HTH_model::Vertex>vertices{
		{{0.0,-0.5f},{1.0,0.0,0.0}},
		{{0.5,0.5},{0.0,1.0,0.0}},
		{{-0.5,0.5},{0.0,0.0,1.0}}
	};
	auto hth_model = std::make_shared<HTH_model>( hth_device,vertices );
	auto triangle = HTH_game_object::createObject();
	triangle.model = hth_model;
	triangle.color = { 1.f,0.5f,1.f };
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = { 2.f,0.5f };
	triangle.transform2d.rotation = 0.25f*glm::two_pi<float>();
	gameObjects.push_back(std::move(triangle));
}
