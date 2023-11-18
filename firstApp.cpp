#include "firstApp.hpp"
#include "Keyboard_movement_controller.hpp"
#include "HTH_camera.hpp"
#include "simple_render_system.hpp"
#include<stdexcept>
#include<chrono>
#include<iostream>
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
    HTH_camera camera{};
    auto viewObject = HTH_game_object::createObject();
    Keyboard_movement_controller movement_controller{};
    camera.setViewDirection(glm::vec3{ -1.f,-1.f,-2.f }, glm::vec3{ 0.f,0.0f,1.0f });
    //camera.setViewTarget(glm::vec3{ -1.f,-1.f,-2.f }, glm::vec3{ 0.0f,0.0f,2.5f });
    auto curren_time = std::chrono::high_resolution_clock::now();
    while (!hth_window.shouldClose())
    {
        glfwPollEvents();

        auto new_time= std::chrono::high_resolution_clock::now();
        float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - curren_time).count();
        curren_time = new_time;

        movement_controller.moveInPlaneXZ(hth_window.getGLFWwindow(), frame_time, viewObject);
        camera.setViewYXZ(viewObject.transform.translation, viewObject.transform.rotation);
        float aspect = hth_renderer.getAspectRatio();
        //std::cout << "aspect =" << aspect << std::endl;
        //camera.setOrthoGraphicProjection(-aspect,aspect, 1.f, -1.f, 1.f, 3.f);

        camera.setPerspectiveProjection(glm::radians(50.f),aspect, 1.f, 10.f);
		auto commandBuffer = hth_renderer.beginFrame();
		if (commandBuffer)
		{
			hth_renderer.beginSwapchainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer,gameObjects,camera);
			hth_renderer.endSwapchainRenderPass(commandBuffer);
			hth_renderer.endFrame();
		}
	}
	vkDeviceWaitIdle(hth_device.device());
}


std::unique_ptr<HTH_model> createCubeModel(HTH_device& device, glm::vec3 offset) {
    std::vector<HTH_model::Vertex> vertices{

        // left face (white)
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<HTH_model>(device, vertices);
}
void firstApp::loadGameObjects()
{
    std::shared_ptr<HTH_model>cubeModel = createCubeModel(hth_device, {0.0f,0.0f,0.0f});
    auto cube = HTH_game_object::createObject();
    cube.model = cubeModel;
    cube.transform.translation = { 0.0f,0.0f,2.5f };
    cube.transform.scale = { 0.5f,0.5f,0.5f };
    gameObjects.push_back(std::move(cube));
    /*
    std::vector<HTH_model::Vertex>vertices{
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
	};
	auto hth_model = std::make_shared<HTH_model>( hth_device,vertices );
	auto triangle = HTH_game_object::createObject();
	triangle.model = hth_model;
	triangle.color = { 1.f,0.5f,1.f };
    triangle.transform.translation = { 0.0f,0.0f,0.5f };
    triangle.transform.scale = { 0.5f,0.5f,0.5f };
	gameObjects.push_back(std::move(triangle));
    */
}
