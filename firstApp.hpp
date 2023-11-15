#pragma once
#include "HTH_device.hpp"
#include "HTH_window.hpp"
#include "HTH_renderer.hpp"
#include "HTH_model.hpp"
#include "HTH_game_object.hpp"

#include<memory>
#include<vector>
class firstApp {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	firstApp();
	~firstApp();
	firstApp(const firstApp&) = delete;
	firstApp &operator=(const firstApp&) = delete;
	void run();
	
private:
	void loadGameObjects();

	HTH_window hth_window{WIDTH, HEIGHT, "Hello Vulkan!"};
	HTH_device hth_device{ hth_window };
	HTH_renderer hth_renderer{ hth_window,hth_device };
	std::vector<HTH_game_object>gameObjects;
};