#pragma once
#include "HTH_model.hpp"
#include<memory>
struct Transform2dComponent {
	glm::vec2 translation{};//position offset
	glm::vec2 scale{ 1.0f,1.0f };
	float rotation;
	glm::mat2 mat2() 
	{
		const float sint = glm::sin(rotation);
		const float cost = glm::cos(rotation);
		glm::mat2 rotationMat2 = { {cost,sint} ,{-sint,cost} };
		glm::mat2 scaleMat2{ {scale.x,0.f},{0.f,scale.y} };
		return rotationMat2*scaleMat2; 
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
	Transform2dComponent transform2d{};
private:
	id_t id;
	HTH_game_object(id_t objId) :id{ objId }
	{

	}

};