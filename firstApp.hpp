#pragma once
#include "HTH_window.hpp"

class firstApp {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	void run();
private:
	HTH_window hth_window{WIDTH, HEIGHT, "Hello Vulkan!"};

};