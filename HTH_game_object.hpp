#pragma once
#include "HTH_model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include<memory>
struct TransformComponent {
	glm::vec3 translation{};//position offset
	glm::vec3 scale{ 1.0f,1.0f,1.0f };
	glm::vec3 rotation{};
	glm::mat4  mat4()
	{
		/*auto transform = glm::translate(glm::mat4{1.f}, translation);
		transform = glm::rotate(transform, rotation.y, { 0.0,1.0f,0.0f });
		transform = glm::rotate(transform, rotation.x, { 1.0,0.0f,0.0f });
		transform = glm::rotate(transform, rotation.z, { 0.0,0.0f,1.0f });
		transform = glm::scale(transform, scale);
		return transform;
		*/
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		return glm::mat4{
			{
				scale.x * (c1 * c3 + s1 * s2 * s3),
				scale.x * (c2 * s3),
				scale.x * (c1 * s2 * s3 - c3 * s1),
				0.0f,
			},
			{
				scale.y * (c3 * s1 * s2 - c1 * s3),
				scale.y * (c2 * c3),
				scale.y * (c1 * c3 * s2 + s1 * s3),
				0.0f,
			},
			{
				scale.z * (c2 * s1),
				scale.z * (-s2),
				scale.z * (c1 * c2),
				0.0f,
			},
			{translation.x, translation.y, translation.z, 1.0f} };
	}
};
class HTH_game_object {
public:

	using id_t = unsigned int;
	static HTH_game_object createObject()
	{
		static id_t current_id = 0;
		return HTH_game_object{ current_id++ };
	}
	id_t getId()
	{
		return id;
	}

	HTH_game_object(const HTH_game_object&) = delete;
	HTH_game_object& operator=(const HTH_game_object&) = delete;
	HTH_game_object(HTH_game_object&&) = default;
	HTH_game_object& operator=(HTH_game_object&&) = default;

	std::shared_ptr<HTH_model>model{};
	glm::vec3 color{};
	TransformComponent transform{};
private:
	id_t id;
	HTH_game_object(id_t objId) :id{ objId }
	{

	}

};