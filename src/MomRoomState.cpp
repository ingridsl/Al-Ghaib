#include "MomRoomState.hpp"
#include "InputManager.hpp"
#include "Camera.hpp"
#include "Collision.hpp"
#include "Walls.hpp"
#include "EmptyBox.hpp"
#include "SceneDoor.hpp"
#include "ScenePortal.hpp"
#include "SceneAnimated.hpp"
#include "MovingObject.hpp"
#include "StealthObject.hpp"

#include <iostream>

MomRoomState::MomRoomState(std::vector<std::unique_ptr<GameObject>> obj, bool inicial, int x, int y) :
	State(), tileSetChao(144, 120, "img/tileSet/tileset_chao.png"),
	tileSetParede(144, 129, "img/tileSet/tileset_mom_parede.png"), tileMapChao("map/tileMapMomChao.txt", &tileSetChao),
	tileMapParede("map/tileMapMomParede.txt", &tileSetParede){

	limits = tileMapChao.FindLimits();
	if(x != -1 && y != -1) SetPlayer(x, y, CAMERA_TYPE1, limits);
	else SetPlayer(200, 400, CAMERA_TYPE1, limits);
	if(inicial){
		//std::cout << "HSC1.1 " << objectArray.size() << std::endl;
		SetInitialObjectArray();
		objectArray.insert( objectArray.end(),
				std::make_move_iterator(obj.begin()),
				std::make_move_iterator(obj.end()) );
	} else{
		//std::cout << "HSC1.2 " << objectArray.size() << std::endl;
		objectArray = std::move(obj);
	}
	objectArray.emplace_back(MissionManager::enemy);
	objectArray.emplace_back(MissionManager::cat);
	objectArray.emplace_back(MissionManager::player);

	RandomState();
	LoadAssets();
	std::cout << "LR2 " << objectArray.size() << std::endl;
}

MomRoomState::~MomRoomState() {
	objectArray.clear();
}

void MomRoomState::Pause(){
}

void MomRoomState::Resume(){
}

void MomRoomState::LoadAssets(){
	bg.Open("img/ocean.jpg");
	background.Open("img/ocean.jpg");
}

void MomRoomState::Update(float dt){
	InputManager instance = InputManager::GetInstance();

	if(instance.KeyPress(ESCAPE_KEY)){
		EndState();
	}
	if(instance.KeyPress(W_KEY)){
		ChangeState("MomRoomState", "StageState");
	}
	quitRequested = instance.QuitRequested();

	Camera::Update(dt);
	UpdateArray(dt);

	int changeIndex = -1;
	posInvert = -1;
	for(int i = objectArray.size() - 1; i >= 0; --i) {
		if(objectArray[i].get()->Is("SceneDoor")){
			//std::cout << "DOOR" << std::endl;
			if(((SceneDoor*)objectArray[i].get())->GetChangeState()){
				changeIndex = i;
			}
		}
		for(int j = i-1; j >= 0; --j){
			if(Collision::IsColliding(objectArray[i].get()->box, objectArray[j].get()->box,
				objectArray[i].get()->rotation*PI/180, objectArray[j].get()->rotation*PI/180)){

				if(objectArray[i].get()->NotifyCollision(*objectArray[j].get())){
					if(i > posInvert) posInvert = i;
				}
				if(objectArray[j].get()->NotifyCollision(*objectArray[i].get())){
					if(j > posInvert) posInvert = j;;
				}

			}
		}
	}

	if(changeIndex != -1){
		((SceneDoor*)objectArray[changeIndex].get())->SetChangeState(false);
		ChangeState("MomRoomState",
				((SceneDoor*)objectArray[changeIndex].get())->GetDest(), 800, 300, (int) Player::SUL);
	}
}

void MomRoomState::Render(){
	background.Render(0,0,0);
	bg.Render(0 - Camera::pos.x, Camera::pos.y+33,0);
	tileMapChao.RenderLayer(0, Camera::pos.x, Camera::pos.y);
	tileMapParede.RenderLayer(0, Camera::pos.x, Camera::pos.y);

	//bg.Render(0,0,0);
	RenderArray();

	if(MissionManager::player->GetShowingInventory()){
		MissionManager::player->RenderInventory();
	} else{
		MissionManager::player->RenderInHand();
		MissionManager::player->RenderNoise();
	}
}

void MomRoomState::SetInitialObjectArray(){
	EmptyBox* EB = new EmptyBox();
	objectArray.emplace_back(EB);

	//MovingObject* Banco = new MovingObject(650, 370, "img/moveis/sala/banquinho.png", true);
	//objectArray.emplace_back(Banco);
}
