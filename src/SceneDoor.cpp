#include "SceneDoor.hpp"
#include "MissionManager.hpp"
#include "Mission.hpp"
#include "Player.hpp"

int SceneDoor::ValorPassar =15;
int SceneDoor::count = 200;

SceneDoor::SceneDoor(float x, float y, std::string dest, bool locked, std::string img, std::string img2) :
	SceneObject(x, y, img, img2, 0, 1, 1, "", SAMEX), dest(dest) {

	lock = locked;
	changeState = false;
	//count =0;
	contador = 0;
}

bool SceneDoor::NotifyCollision(GameObject& other){
	if(!(other.Is("EmptyBox") || other.Is("Bear") || other.Is("Enemy"))){
		SceneObject::NotifyCollision(other);
		return false;
	}

	if(other.Is("Bear") || other.Is("Enemy")){
		count ++;
		//std::cout << "teste door" << count << std::endl;
		if(count == 1)
			sp.Open(change2);
		if(count == ValorPassar)
			sp.Open(change1);
		int w = box.w;
		box.x += box.w - sp.GetWidth();
		box.w = sp.GetWidth();
		box.h = sp.GetHeight();

		MovePlayerColliding(w, box.h);
	}

	if(other.Is("EmptyBox")){
		if(InputManager::GetInstance().KeyPress(Z_KEY) && lock == true){
				MissionManager::player->SetDoor(true);
		}
		if((InputManager::GetInstance().KeyPress(Z_KEY) && lock == false)){
			if(estado){
				changeState = true;
			} else{
				estado = true;
				sp.Open(change2);
				int w = box.w;
				box.x += box.w - sp.GetWidth();
				box.w = sp.GetWidth();
				box.h = sp.GetHeight();

				MovePlayerColliding(w, box.h);
			}
		}
	}

	return false;
}

bool SceneDoor::Is(std::string type){
	return (type == "SceneDoor" || type == "CollidableObject");
}

bool SceneDoor::ReceiveAction(InventoryObject* other){
	if(other->IsObject("InventoryKey")){
		if(lock == true){
			lock = false;
			return true;
		}
	}
	return false;
}

bool SceneDoor::GetChangeState(){
	return changeState;
}

void SceneDoor::SetChangeState(bool change){
	changeState = change;
}

std::string SceneDoor::GetDest(){
	return dest;
}
