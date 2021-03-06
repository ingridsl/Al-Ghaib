#include "State.hpp"
#include "MissionManager.hpp"
#include "SceneObject.hpp"
#include "SceneAnimated.hpp"
#include "Camera.hpp"

#include <random>
#include <iostream>
#include <chrono>

State::State(){
	quitRequested = false;
	popRequested = false;
	posInvert = -1;
	endGame = false;
}

State::~State(){
	objectArray.clear();
}

void State::AddObject(GameObject *object){
	objectArray.emplace_back(object);
}

void State::UpdateArray(float dt){
	for(int i = objectArray.size() - 1; i >= 0; --i) {
		objectArray[i].get()->Update(dt);
		if(objectArray[i].get()->IsDead()){
			objectArray.erase(objectArray.begin() + i);
		}
	}
}

void State::RenderArray(){
	if(posInvert != -1){
		for(unsigned int i = 0; i < objectArray.size(); i++) {
			if((int) i != posInvert){
				objectArray[i].get()->Render();
			}
		}
		objectArray[posInvert].get()->Render();
	} else{
		for(unsigned int i = 0; i < objectArray.size(); i++) {
			objectArray[i].get()->Render();
		}
	}
}

bool State::QuitRequested(){
	return quitRequested;
}

bool State::PopRequested(){
	return popRequested;
}

void State::SetPlayer(int x, int y, int type, Rect limits){
	Camera::Unfollow();
	MissionManager::player->SetPosition(x, y);
	MissionManager::player->SetMovementLimits(limits);
	if(type != CAMERA_TYPE0 && type != CAMERA_TYPE3) Camera::Follow(MissionManager::player, type);
	else Camera::SetType(type);
}

void State::RemoveAll(){
	for(unsigned int i = 0; i < objectArray.size(); ) {
		if(objectArray[i].get()->Is("Player")){
			//std::cout << "REMOVE PLAYER" << std::endl;
			//std::cout << "remove " << objectArray.size() << " " << i << std::endl;
			MissionManager::player = (Player*) objectArray[i].release();
			objectArray.erase(objectArray.begin() + i);
			//std::cout << "alive " << objectArray.size() << " " << MissionManager::player->Is("Player") << std::endl;
		} else if(objectArray[i].get()->Is("Enemy")){
			//std::cout << "REMOVE ENEMY" << std::endl;
			//std::cout << "remove " << objectArray.size() << " " << i << std::endl;
			MissionManager::enemy = (Enemy*) objectArray[i].release();
			objectArray.erase(objectArray.begin() + i);
			//std::cout << "alive " << objectArray.size() << " " << MissionManager::player->Is("Enemy") << std::endl;
		} else if(objectArray[i].get()->Is("Cat")){
			//std::cout << "REMOVE CAT" << std::endl;
			//std::cout << "remove " << objectArray.size() << " " << i << std::endl;
			MissionManager::cat = (Cat*) objectArray[i].release();
			objectArray.erase(objectArray.begin() + i);
			//std::cout << "alive " << objectArray.size() << " " << MissionManager::player->Is("Cat") << std::endl;
		} else i++;
	}
}

void State::RemoveEnemy(){
	for(unsigned int i = 0; i < objectArray.size(); i++) {
		if(objectArray[i].get()->Is("Enemy")){
			//std::cout << "remove " << objectArray.size() << " " << i << std::endl;
			MissionManager::enemy = (Enemy*) objectArray[i].release();
			objectArray.erase(objectArray.begin() + i);
			//std::cout << "alive " << objectArray.size() << " " << MissionManager::player->Is("Enemy") << std::endl;
		}
	}
}

void State::SetPopRequested(){
	popRequested = true;
}

Rect State::GetStateLimits(){
	return limits;
}

void State::RandomState(){
	if(MissionManager::missionManager->randomStates){
		std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<int> distribution(1,6);

		for(unsigned int i = 0; i < objectArray.size(); i++) {
			if(objectArray[i].get()->Is("SceneObject") && !objectArray[i].get()->Is("SceneAnimated")){
				if(!((SceneObject*) objectArray[i].get())->GetBlockRandom()){
					int dice_roll = distribution(generator);
					//std::cout << "DICE " << dice_roll << std::endl;
					if(dice_roll <= 2){
						((SceneObject*) objectArray[i].get())->ChangeImage();
					}
				}
			}
		}
	}
}

int State::GetPositionArray(std::string obj){
	for(unsigned int i = 0; i < objectArray.size(); i++) {
		if(objectArray[i].get()->Is(obj)){
			return i;
		}
	}
	return -1;
}

void State::AccessAnimated(int pos){
	((SceneAnimated*)objectArray[pos].get())->ChangeImage();
}

void State::EndState(){
	std::cout << "ES1" << std::endl;
	MissionManager::player->SetBlocked(false);
	MissionManager::player->SetBloqHUD(false);
	MissionManager::player->SetBloqInv(false);
	std::cout << "ES2" << std::endl;
	Camera::Unfollow();
	Camera::DontMove();
	RemoveAll();
	std::cout << "ES3" << std::endl;
	popRequested = true;
	std::cout << "ES4" << std::endl;
}

void State::ChangeState(std::string orig, std::string dest, int x, int y, int dir){
	EndState();
	Game::GetInstance().GetMissionManager().
			ChangeState(std::move(objectArray), orig, dest, x, y, dir);
}

void State::ChangeMission(int num){
	EndState();
	std::vector<std::string> inventory = MissionManager::player->GetStringInventory();
	Game::GetInstance().GetMissionManager().
			ChangeMission(num, MissionManager::player->GetInHandIndex(), inventory);
}

std::vector<std::pair<int,int>> State::GetPosVector(){
	std::vector<std::pair<int,int>> aux;
	for(unsigned int i = 0; i < objectArray.size() - 2; i++){
		aux.push_back(std::make_pair(objectArray[i].get()->box.x, objectArray[i].get()->box.y));
	}
	return aux;
}

void State::SetEndGame(){
	endGame = true;
}
