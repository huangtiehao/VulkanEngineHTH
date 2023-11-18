#include "Keyboard_movement_controller.hpp"

void Keyboard_movement_controller::moveInPlaneXZ(GLFWwindow* window, float dt, HTH_game_object& gameObject)
{
	glm::vec3 rotate{ 0.f };
	if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)rotate.y += 1.f;
	if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)rotate.y -= 1.f;
	if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)rotate.x += 1.f;
	if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)rotate.x -= 1.f;
	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
	}
	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = gameObject.transform.rotation.y;
	glm::vec3 forwardDir{ sin(yaw),0.f,cos(yaw) };
	glm::vec3 rightDir{ cos(yaw),0.0f,-sin(yaw) };
	glm::vec3 upDir{ 0.0f,-1.0f,0.0f };

	glm::vec3 move{ 0.0f };
	if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)move += forwardDir;
	if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)move -= forwardDir;
	if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)move += rightDir;
	if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)move -= rightDir;
	if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)move += upDir;
	if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)move -= upDir;

	if (glm::dot(move, move) > std::numeric_limits<float>::epsilon())
	{
		gameObject.transform.translation += moveSpeed * dt * glm::normalize(move);
	}
}
