#include "LivingRoomState.hpp"
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
#include "SceneUntouchable.hpp"

#include <iostream>

LivingRoomState::LivingRoomState(std::vector<std::unique_ptr<GameObject>> obj, bool inicial, int x, int y) :
	State(), tileSetChao(150, 117, "img/tileSet/tile_sala_chao.png"),
	tileSetParede(150, 126, "img/tileSet/tile_sala_parede.jpeg"), tileMapChao("map/tileMapSalaChao.txt", &tileSetChao),
	tileMapParede("map/tileMapSalaParede.txt", &tileSetParede){

	limits = tileMapChao.FindLimits();
	if(x != -1 && y != -1) SetPlayer(x, y, CAMERA_TYPE3, limits);
	else SetPlayer(250, 250, CAMERA_TYPE3, limits);
	std::cout << "Player: " << MissionManager::player->box.x << " " << MissionManager::player->box.y << std::endl;

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
	objectArray.emplace_back(MissionManager::cat);
	objectArray.emplace_back(MissionManager::enemy);
	objectArray.emplace_back(MissionManager::player);

	music = Music("audio/ambiente.wav");

	RandomState();
	LoadAssets();
	std::cout << "LR2 " << objectArray.size() << std::endl;
}

LivingRoomState::~LivingRoomState() {
	objectArray.clear();
}

void LivingRoomState::Pause(){
	music.Stop();
}

void LivingRoomState::Resume(){
	music.Play(-1);
}

void LivingRoomState::LoadAssets(){
	bg.Open("img/HUD/blackBack.jpg");
	background.Open("img/HUD/blackBack.jpg");
}

void LivingRoomState::Update(float dt){
	InputManager instance = InputManager::GetInstance();

	if(instance.KeyPress(ESCAPE_KEY)){
		EndState();
	}
	if(instance.KeyPress(Q_KEY)){
		ChangeState("LivingRoomState", "StageState");
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
		ChangeState("LivingRoomState",
				((SceneDoor*)objectArray[changeIndex].get())->GetDest(), 520, 200, (int) Player::SUL);
	}

	/*if(MissionManager::missionManager->setObjectTile){
		tileMapChao.AddObjects();
		MissionManager::missionManager->setObjectTile = false;
	}*/

}

void LivingRoomState::Render(){
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

void LivingRoomState::SetInitialObjectArray(){
	EmptyBox* EB = new EmptyBox();
	objectArray.emplace_back(EB);

	ScenePortal* DoorToLivingRoom = new ScenePortal(220, 165, "HallState");
	objectArray.emplace_back(DoorToLivingRoom);

	SceneObject* Sofa = new SceneObject(500, 330, "img/cenario/sala/sofa.png", "");
	objectArray.emplace_back(Sofa);

	SceneObject* Poltrona = new SceneObject(400, 355, "img/cenario/sala/poltrona.png", "");
	objectArray.emplace_back(Poltrona);

	SceneObject* Mesa = new SceneObject(500, 400, "img/cenario/sala/mesa.png", "");
	objectArray.emplace_back(Mesa);

	SceneAnimated* TV = new SceneAnimated(650, 350, "img/cenario/sala/televisao_desligada.png", "img/cenario/sala/televisao.png");
	objectArray.emplace_back(TV);

	if(MissionManager::player->HaveObject("InventoryControl")){
		SceneObject* Criado = new SceneObject(735, 390, "img/cenario/filho/criado-fechado.png",
					"img/cenario/filho/criado-aberto.png", 0, 1, 1, "", SceneObject::SAMEY_UP);
		objectArray.emplace_back(Criado);
	} else{
		SceneObject* Criado = new SceneObject(735, 390, "img/cenario/filho/criado-fechado.png",
				"img/cenario/filho/criado-aberto.png", 0, 1, 1, "InventoryControl", SceneObject::SAMEY_UP);
		objectArray.emplace_back(Criado);
	}

	SceneObject* Painel = new SceneObject(390, 127, "img/cenario/sala/mosaico.png", "");
	objectArray.emplace_back(Painel);

	SceneUntouchable* Pilastra1 = new SceneUntouchable(350, 127, "img/cenario/sala/pilastra.png");
	objectArray.emplace_back(Pilastra1);

	SceneUntouchable* Pilastra2 = new SceneUntouchable(725, 127, "img/cenario/sala/pilastra.png");
	objectArray.emplace_back(Pilastra2);

	SceneObject* Armario = new SceneObject(790, 180,
			"img/cenario/geral/armario-corredor-fechado.png", "img/cenario/geral/armario-corredor-aberto.png");
	objectArray.emplace_back(Armario);
}
