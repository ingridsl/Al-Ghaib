#include "Mission1.hpp"
#include "PickUpObject.hpp"
#include "SceneWindow.hpp"
#include "StealthObject.hpp"
#include "SceneDoor.hpp"

#include <iostream>

Mission1::Mission1(): blackSquare("img/blacksquare.png") {
	initialState = "StageState";
	initialX = 600; initialY = 400;
	MissionManager::missionManager->SetPos(initialX, initialY, initialState);

	MissionManager::CountHallState = 0;
	MissionManager::CountStageState = 0;

	//StageState = 0;
	//HallState = 0;
	trancada = false;
	begin = true;

	SDL_Color auxcolor = SDL_Color();
	auxcolor.r = 102;
	auxcolor.g = 0;
	auxcolor.b = 0;

	flagTimer = true; //stopMusic = false;
	time = Timer();
	cooldown = Timer();
	tx = Text("font/uwch.ttf", 80, Text::TextStyle::BLENDED, "MISSAO 1", auxcolor, 0, 0);
	tx.SetPos(0, 0, true, true);

	falas = Text("font/AA_typewriter.ttf", 30, Text::TextStyle::BLENDED , "A NOITE É FRIA E PERIGOSA", auxcolor, 0, 0);
	falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
	ultimoTempo = 3;
	/*intro = Music("audio/menu-intro.wav");
	music = Music("audio/menu-loop.wav");
	intro.Play(1);
	Mix_HookMusicFinished(&TitleState::LinkMusic);*/

	SetObjectStage();
	SetObjectHall();
	state = MissionManager::changeState;

	//std::cout << "INIT_MIS1" << std::endl;
}

Mission1::~Mission1() {

}

void Mission1::SetObjectStage(){
	SceneWindow* Window = new SceneWindow(350, 200, "img/closedwindow.png", "img/openwindow.png");
	objectStage.emplace_back(Window);
	StealthObject* Stealth = new StealthObject(700, 500, "img/closedwindow.png");
	objectStage.emplace_back(Stealth);
	PickUpObject* PO = new PickUpObject(700, 300, "InventoryKey", "img/minionbullet1.png");
	objectStage.emplace_back(PO);
}

void Mission1::SetObjectHall(){
	SceneWindow* Window = new SceneWindow(350, 70, "img/closedwindow.png", "img/openwindow.png");
	objectHall.emplace_back(Window);
	PickUpObject* PO = new PickUpObject(500, 400, "InventoryKey", "img/minionbullet1.png");
	objectHall.emplace_back(PO);
	Enemy* E = new Enemy(500, 70);
	objectHall.emplace_back(E);
}

void  Mission1::Update(float dt){

	InputManager instance = InputManager::GetInstance();
	bool trancada = false;
	if(instance.KeyPress(ESCAPE_KEY)){
		popRequested = true;
	}
	//quitRequested = instance.QuitRequested();
	if(instance.KeyPress(ENTER_KEY)){
		std::cout << "ENTER PRESSED" << std::endl;
		if(time.Get() < 3 ){
			time.Set(3);
			begin = false;
		}
	}
	time.Update(dt);
	cooldown.Update(dt);
	if(time.Get() > 3 ){
		begin = false;
	}

	if(MissionManager::missionManager->GetStage("StageState") && MissionManager::CountStageState <= 1){
		//StageState++;
		//std::cout << "StageState" << std::endl;
		if(flagTimer == true && time.Get() > 3){
			tx.SetText(" ");
			//time.Restart();
			flagTimer = false;
		}
		if( time.Get() > 5.5 && trancada == false && cooldown.Get() > 2/* && ultimoTempo < 5.5*/){
			falas.SetText("ENCONTRE SEU AMIGO QUE O PROTEGE DOS PERIGOS DA NOITE");
			falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
			ultimoTempo = 5.5;

			//flagTimer = true;
		}

		if( time.Get() > 7 && trancada == false && cooldown.Get() > 2/* && ultimoTempo < 7 && ultimoTempo > 5.5*/){
			falas.SetText(" ");
			ultimoTempo = 7;
		}

		MessageDoor(dt);
	} else if(MissionManager::missionManager->GetStage("HallState") && MissionManager::CountHallState <= 1){
		//HallState++;
		//std::cout << "HallState" << std::endl;
		if(state != MissionManager::changeState){
			state = MissionManager::changeState;
			time.Restart();
		}
		if(trancada == false)
			falas.SetText("CUIDADO");
			falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
			ultimoTempo = 0;
		if(time.Get() > 3 && trancada == false){
			falas.SetText(" ");
			falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
			ultimoTempo = 3;
		}
		if(time.Get()>6 && trancada == false){
			Enemy::show = true;
		}

		MessageDoor(dt);
	} else if(MissionManager::missionManager->GetStage("StageState") && MissionManager::CountStageState > 1){
		if(state != MissionManager::changeState){
					state = MissionManager::changeState;
					time.Restart();
		}
		if(trancada == false){
			falas.SetText("CERTEZA QUE QUER PASSAR A NOITE SEM PROTEÇÃO?");
			falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
		}
		if(time.Get() > 3 && trancada == false){
			falas.SetText(" ");
			falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
		}
		MessageDoor(dt);
	}

}
void Mission1::MessageDoor(float dt){
	if(MissionManager::player->GetDoor() && trancada == false){
				falas.SetText("ESTÁ TRANCADA");
				falas.SetPos(0, Game::GetInstance().GetHeight()-50, true, false);
				trancada = true;
				MissionManager::player->SetDoor(false);
				cooldown.Restart();
				time.Restart();
				while(time.Get()< ultimoTempo){
					time.Update(dt);
				}
			}


			if(cooldown.Get() > 2 && trancada == true){
				//cooldown.Restart();
				falas.SetText(" ");
				trancada = false;
				cooldown.Restart();
				time.Restart();
				while(time.Get()< ultimoTempo){
					time.Update(dt);
				}
			}

}
void  Mission1::Render(){
	//printf("teste2 \n");
	if(/*time.Get() < 3*/ begin ){
		blackSquare.Render(0, 0, 0);
		tx.Render(0,0);
	}

	if(MissionManager::missionManager->GetStage("StageState") && MissionManager::CountStageState <= 1 && time.Get() > 4){
		falas.Render(0,0);
	}
	else{
		falas.Render(0,0);
	}

}

void  Mission1::Pause(){
}

void  Mission1::Resume(){
}
