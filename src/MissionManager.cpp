#include "MissionManager.hpp"
#include "Game.hpp"
#include "InventoryObject.hpp"
#include "StageState.hpp"
#include "HallState.hpp"
#include "Mission1.hpp"
#include "Mission2.hpp"

#include <iostream>

Player* MissionManager::player = nullptr;
MissionManager* MissionManager::missionManager = nullptr;
int MissionManager::CountStageState = 0;
int MissionManager::CountHallState = 0;
int MissionManager::changeState = 0;

//Colocar nos objetos todas as condicoes deles, inclusive se estao abertos ou fechados

MissionManager::MissionManager() {
	numMission = 0;
	mission = nullptr;
	initStage = initHall = true;
	missionManager = this;
	xStage = yStage = xHall = yHall = -1;
	 /*CountStageState = 0;
	CountHallState = 0;
	changeState = false;*/
}

MissionManager::~MissionManager() {
	objectStage.clear();
	objectHall.clear();
	if(mission != nullptr) delete mission;
	player = nullptr;
	missionManager = nullptr;
}

void MissionManager::SetObject(std::vector<std::unique_ptr<GameObject>> objNew, std::string orig){
	if(orig == "StageState"){
		std::vector<std::unique_ptr<GameObject>>().swap(objectStage);
		objectStage = std::move(objNew);
	} else if(orig == "HallState"){
		std::vector<std::unique_ptr<GameObject>>().swap(objectHall);
		objectHall = std::move(objNew);
	}
}

void MissionManager::SetState(std::string dest){
	//inicial serve para indicar se e a 1a vez que o State esta sendo construido
	player->ResetWallLimits();
	if(dest == "StageState"){
		std::cout << "SS.1" << std::endl;
		std::cout << "SIZE: " << objectStage.size() << std::endl;
		Game::GetInstance().Push(new StageState(std::move(objectStage), initStage, xStage, yStage));
		initStage = false;
		stage = "StageState";
		CountStageState++;
		changeState++;
		//std::cout << "ESSE AQUI OH: " << stage << std::endl;
		std::cout << "SS.2" << std::endl;
	} else if(dest == "HallState"){
		std::cout << "HS.1" << std::endl;
		std::cout << "SIZE: " << objectStage.size() << std::endl;
		Game::GetInstance().Push(new HallState(std::move(objectHall), initHall, xHall, yHall));
		initHall = false;
		stage = "HallState";
		//std::cout << "ESSE AQUI OH 2: " << stage << std::endl;
		std::cout << "HS.2" << std::endl;
		CountHallState++;
		changeState++;
	}
}

//quando for chamar pelos estados para mudar de State, usar esse
void MissionManager::ChangeState(std::vector<std::unique_ptr<GameObject>> objNew, std::string orig, std::string dest, int x, int y){
	SetPos(x, y, orig);
	//std::cout << "1" << std::endl;
	SetObject(std::move(objNew), orig);
	//std::cout << "2" << std::endl;
	SetState(dest);
	//std::cout << "3" << std::endl;
}

void MissionManager::SetMission(){
	//Mission e um abstract para as Mission1,2,3...
	//if(mission != nullptr) delete mission;
	switch(numMission){
		case 1:
			mission = new Mission1();
			break;
		case 2:
			mission = new Mission2();
			break;
		default:
			break;
	}

	Game::GetInstance().storedMission = mission;

	//SWAP efetivamente libera a memoria ao inves de clear, pq nao necessariamente estarao vazios
	//objectStage.clear(); objectHall.clear();
	std::vector<std::unique_ptr<GameObject>>().swap(objectStage);
	std::vector<std::unique_ptr<GameObject>>().swap(objectHall);
	//std::cout << "INI_MIS1: " << objectStage.size() << " " << objectHall.size() << std::endl;
	initStage = initHall = true;
	objectStage = std::move(mission->GetObjectStage());
	objectHall = std::move(mission->GetObjectHall());
	//std::cout << "INI_MIS2: " << objectStage.size() << " " << objectHall.size() << std::endl;
}

Mission *MissionManager::GetMission(){
	return mission;
}

//em caso de vitoria, especificado em cada missao
void MissionManager::ChangeMission(int num, int oldInHand, std::vector<std::string> oldInventory){
	bool firstPlay = true;
	numMission = num;
	xStage = yStage = xHall = yHall = -1;
	if(player != nullptr){
		firstPlay = false;
		SaveMission();
	}
	player = new Player(0, 0, oldInHand, oldInventory);
	oldInventory.clear();
	SetMission();
	SetState(mission->GetInitialState());
	if(firstPlay){
		SaveMission();
	}
}

//Ver para liberar memoria dos dados e do player quando vai para o Menu
void MissionManager::DeleteStates(){
	//delete player;
	//std::vector<std::unique_ptr<GameObject>>().swap(objectStage);
	//std::vector<std::unique_ptr<GameObject>>().swap(objectHall);
	std::cout << "Player NULL" << std::endl;
}

void MissionManager::LoadMission(int num){
	std::fstream load;
	std::string name = "saves/save";
	if(num != 0) name.append(std::to_string(num));
	name.append(".txt");
	load.open(name, std::fstream::in);
	if(load.is_open()){
		int numLoadMission, oldInHand;
		load >> numLoadMission;
		load >> oldInHand;
		std::vector<std::string> inventory;
		while(!load.eof()){
			std::string obj;
			load >> obj;
			if(!obj.empty()){
				inventory.emplace_back(obj);
			}
		}
		Game::GetInstance().GetMissionManager().ChangeMission(numLoadMission, oldInHand, inventory);
		inventory.clear();
		load.close();
	} else std::cout << "Nao foi possivel abrir o arquivo." << std::endl;
}

//Salva a missao ao comeca-la, guardando seu estado inicial
void MissionManager::SaveMission(){
	std::fstream save, saveMission;
	save.open("saves/save.txt", std::fstream::out);
	saveMission.open("saves/save" + std::to_string(numMission) + ".txt", std::fstream::out);
	if(save.is_open() && saveMission.is_open()){
		save << numMission << std::endl;
		saveMission << numMission << std::endl;
		save << player->GetInHandIndex() << std::endl;
		saveMission << player->GetInHandIndex() << std::endl;
		std::vector<InventoryObject*> inventory = player->GetInventory();
		for(unsigned int i = 0; i < inventory.size(); i++){
			save << inventory[i]->GetObject() << std::endl;
			saveMission << inventory[i]->GetObject() << std::endl;
		}
		inventory.clear();
		save.close();
		saveMission.close();
	} else std::cout << "Nao foi possivel abrir o arquivo." << std::endl;
}

bool MissionManager::GetStage(std::string type){
	return (type == stage);
}

void MissionManager::SetPos(int x, int y, std::string local){
	if(local == "StageState"){
		xStage = x; yStage = y;
	} else if(local == "HallState"){
		xHall = x; yHall = y;
	}
}
