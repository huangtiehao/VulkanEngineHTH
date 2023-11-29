#include "firstApp.hpp"
#include "Keyboard_movement_controller.hpp"
#include "HTH_camera.hpp"
#include "HTH_buffer.hpp"
#include "simple_render_system.hpp"
#include "point_light_system.hpp"
#include<stdexcept>
#include<chrono>
#include<iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>
#include<array>

struct GlobalUbo {
    glm::mat4 projection{ 1.f };
    glm::mat4 view{ 1.f };
    glm::vec4 ambientLightColor{ 1.0f,1.0f,1.0f,0.02f };
    glm::vec4 lightPosition = glm::vec4{0.f,-0.5f,0.f,1.0f};
    glm::vec4 lightColor{ 1.0f };
};
firstApp::firstApp()
{
    globalPool = HTH_descriptorPool::Builder(hth_device)
        .setMaxSets(HTH_swap_chain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,HTH_swap_chain::MAX_FRAMES_IN_FLIGHT).build();
	loadGameObjects();
}
firstApp::~firstApp()
{
	
}
void firstApp::run()
{
    std::vector<std::unique_ptr<HTH_buffer>> uboBuffers(HTH_swap_chain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<HTH_buffer>(
            hth_device,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

    auto globalSetLayout = HTH_descriptorSetLayout::Builder(hth_device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT).build();
    std::vector<VkDescriptorSet>globalDescriptorSets(HTH_swap_chain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < globalDescriptorSets.size(); ++i)
    {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        HTH_descriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

	Simple_render_system simpleRenderSystem{ hth_device, hth_renderer.getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout()};
    Point_light_system pointLightSystem{ hth_device, hth_renderer.getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout() };
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

        camera.setPerspectiveProjection(glm::radians(50.f),aspect, 0.01f, 10.f);
		auto commandBuffer = hth_renderer.beginFrame();
		if (commandBuffer)
		{
            int frameIndex = hth_renderer.getFrameIndex();
            HTH_frame_info frameInfo{frameIndex,frame_time,commandBuffer,camera,globalDescriptorSets[frameIndex],gameObjects};
            GlobalUbo ubo{};
            ubo.projection = camera.getProjectionMatrix();
            ubo.view = camera.getViewMatrix();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();


			hth_renderer.beginSwapchainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(frameInfo);
            pointLightSystem.render(frameInfo);
			hth_renderer.endSwapchainRenderPass(commandBuffer);
			hth_renderer.endFrame();
		}
	}
	vkDeviceWaitIdle(hth_device.device());
}


void firstApp::loadGameObjects()
{
    std::shared_ptr<HTH_model>loaded_model1 = HTH_model::createModelFromFile(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/models/flat_vase.obj");
    auto gameObject1 = HTH_game_object::createObject();
    gameObject1.model = loaded_model1;
    gameObject1.transform.translation = { 0.f,0.f,1.5f };
    gameObject1.transform.scale = glm::vec3{2.f,2.f,2.f };
    gameObjects.emplace(gameObject1.getId(),std::move(gameObject1));

    std::shared_ptr<HTH_model>loaded_model2 = HTH_model::createModelFromFile(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/models/flat_vase.obj");
    auto gameObject2 = HTH_game_object::createObject();
    gameObject2.model = loaded_model2;
    gameObject2.transform.translation = { -1.0f,0.0f,1.0f };
    gameObject2.transform.scale = glm::vec3{ 2.f,2.f,2.f };
    gameObjects.emplace(gameObject2.getId(), std::move(gameObject2));

    std::shared_ptr<HTH_model>loaded_model3 = HTH_model::createModelFromFile(hth_device, "C:/visual_studio_source_code/VulkanEngineHTH/models/quad_floor.obj");
    auto gameObject3 = HTH_game_object::createObject();
    gameObject3.model = loaded_model3;
    gameObject3.transform.translation = { 0.0f,0.0f,0.0f };
    gameObject3.transform.scale = glm::vec3{ 2.f,2.f,2.f };
    gameObjects.emplace(gameObject3.getId(), std::move(gameObject3));
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
