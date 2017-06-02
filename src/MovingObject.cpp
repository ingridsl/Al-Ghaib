#include "MovingObject.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include "Geometry.hpp"

MovingObject::MovingObject(float x, float y, std::string img) : sp(img){
	rotation = 0;
	box.x = x; box.y = y;
	box.w = sp.GetWidth();
	box.h = sp.GetHeight();
	previousPos = Vec2(x,y);
}

bool MovingObject::IsDead(){
	return false;
}

void MovingObject::Update(float dt){

}

void MovingObject::Render(){
	sp.Render(box.x - Camera::pos.x, box.y - Camera::pos.y, rotation);
}

void MovingObject::NotifyCollision(GameObject& other){
	if(other.Is("EmptyBox")){
		if(InputManager::GetInstance().IsKeyDown(Z_KEY)){
			previousPos = Vec2(box.x, box.y);

			if(box.x + Player::player->GetSpeed().x < limits.w - box.w && box.x + Player::player->GetSpeed().x > limits.x){
				box.x += Player::player->GetSpeed().x;
				if((Player::player->box).Intersect(box)) box.x -= Player::player->GetSpeed().x;
			}
			if(box.y + Player::player->GetSpeed().y < limits.h - box.h && box.y + Player::player->GetSpeed().y > limits.y){
				box.y += Player::player->GetSpeed().y;
				if((Player::player->box).Intersect(box)) box.y -= Player::player->GetSpeed().y;
			}
		}
	}

	if(other.Is("Player")){
		if(Player::player->box.x < box.x + box.w ||
				Player::player->box.x + Player::player->box.w > box.x){
			Player::player->box.x = Player::player->previousPos.x;
		}
		if(Player::player->box.y < box.y + box.h ||
				Player::player->box.y + Player::player->box.h > box.y){
			Player::player->box.y = Player::player->previousPos.y;
		}
	}

	if(other.Is("SceneObject") || other.Is("SceneWindow") ||
			other.Is("SceneDoor") || other.Is("MovingObject")/* || other.Is("Walls")*/){
		box.x = previousPos.x;
		box.y = previousPos.y;
	}

	if(other.Is("Walls")){
		if(box.x < other.box.x + other.box.w ||
				box.x + box.w > other.box.x){
					box.x = previousPos.x;
		}
		if(box.y < other.box.y + other.box.h ||
				box.y + box.h > other.box.y){
				box.y = previousPos.y;
		}
	}

	if (other.Is("Enemy")){
		if(other.box.x < box.x + box.w ||
				other.box.x + other.box.w > box.x){
			other.box.x = box.x + box.w + 1;
		} else if(other.box.y < box.y + box.h ||
				other.box.y + other.box.h > box.y){
			other.box.x = box.x - other.box.w - 1;
		}
	}
}

void MovingObject::SetMovementLimits(Rect limits){
	this->limits.x = limits.x;
	this->limits.y = limits.y;
	this->limits.w = limits.w;
	this->limits.h = limits.h;
}

bool MovingObject::Is(std::string type){
	return (type == "MovingObject");
}

