#include "Mission2.hpp"
#include "PickUpObject.hpp"
#include "StealthObject.hpp"
#include "SceneDoor.hpp"
#include "MissionManager.hpp"

 Music Mission2::music;

Mission2::Mission2() : Mission(), parado(false) {
	initialState = "StageState";
	initialX = 450; initialY = 400;
	MissionManager::missionManager->SetPos(initialX, initialY);

	SDL_Color redwine = SDL_Color();
		redwine.r = 102;
		redwine.g = 0;
		redwine.b = 0;

		SDL_Color white = SDL_Color();
		white.r = 255;
		white.g = 255;
		white.b = 255;

	tx = Text("font/uwch.ttf", 50, Text::TextStyle::BLENDED, "NOITE 2", redwine, 0, 0);
	tx.SetPos(0, 0, true, true);
	creepy = Text("font/uwch.ttf", 30, Text::TextStyle::BLENDED, "\"fechai as portas, mantende crianças por perto à noite...\"", redwine, 0, 0);
	creepy.SetPos(0, Game::GetInstance().GetHeight()-120, true, false);

	falas = Text("font/AA_typewriter.ttf", 25, Text::TextStyle::BLENDED , " ", white, 0, 0);
	falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
	showBox = false;
	//ultimoTempo = 3;
	/*intro = Music("audio/menu-intro.wav");
	music = Music("audio/menu-loop.wav");
	intro.Play(1);
	Mix_HookMusicFinished(&TitleState::LinkMusic);*/

	//Sound intro("audio/boom.wav");
	//intro.Play(1);

	SetObjectStage();
	SetObjectHall();
}

Mission2::~Mission2() {

}

void  Mission2::Pause(){ }

void  Mission2::Resume(){ }

void Mission2::Update(float dt){

	InputManager instance = InputManager::GetInstance();
	bool trancada = false;
	if(instance.KeyPress(ESCAPE_KEY)){
		popRequested = true;
	}
	//quitRequested = instance.QuitRequested();
	if(instance.KeyPress(SPACE_KEY)){
		std::cout << "SPACE KEY PRESSED" << std::endl;
		if(time.Get() < 3){
			time.Set(3);
			begin = false;
		}
		bloqBlack = true;
		fadeIn = false;
		alpha = TRANSPARENT;
		if (spFade.IsOpen()) {
			spFade.ChangeAlpha(alpha);
		}
	}
	time.Update(dt);
	cooldown.Update(dt);
	//std::cout << "time: " << time.Get() << std::endl;
	if(time.Get() > 6){
		begin = false;
		fadeIn = false;
	}

	/*if(endMission && time.Get() > (12*0.25 + 0.5)){
		MissionManager::player->SetBlocked(false);
		Game::GetInstance().GetCurrentState().SetPopRequested();
		Game::GetInstance().GetMissionManager().ChangeMission(2);
	}*/
	//URSO APARECE BATENDO NA PORTA. BOTAR SOM DE PORTA TENTANDO ABRIR ANTES DE ELE FALAR
	if(MissionManager::missionManager->GetStage("StageState") &&
			MissionManager::missionManager->countStageState <= 1){
			if(flagTimer == true && time.Get() > 3){
				tx.SetText(" ");
				showBox = false;
				creepy.SetText(" ");
				flagTimer = false;
			}
			if(time.Get() > 5 && time.Get() < 6 && trancada == false && cooldown.Get() > 3){
				Sound portaDestrancando = Sound ("audio/weird-door.wav");
				portaDestrancando.Play(0);
			}
				//falas.SetText("U: EI.... AQUI... ABRE PRA MIM.");
				//falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
				//ultimoTempo = 4;
				//time.Restart();
				//showBox = true;
				//flagTimer = false;
			//}
			//if( time.Get() > 10 && trancada == false && cooldown.Get() > 2){
				//falas.SetText("U: ME... AJUDA...");
				//falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
				//ultimoTempo = 10;
				//showBox = true;
				//flagTimer = true;
			//}

			//if( time.Get() > 14 && trancada == false && cooldown.Get() > 2){
			//	falas.SetText(" "); //PARA FAZER TEXTO DESAPARECER. N PODE DEIXAR SEM ESPAÃ‡O DENTRO QUE DÃ� ERRO
			//	ultimoTempo = 14;
			//	showBox = false;
			//}
			if(time.Get() > 12 && trancada == false && cooldown.Get() > 2){
				Bear::show = true;
				count ++;
				//std::cout << "test 0 " << std::endl;
				if(count == 1){
					parado = true;
					Bear::bear->SetDestinationPath(Vec2(805, 310)); //2º DESTINO
					Bear::bear->SetDestinationPath(Vec2(805, 210)); //1º DESTINO
					ultimoTempo = 12;
				}
				if(parado){
					Bear::bear->SetDestinationPath(Vec2(805, 310));
				}
				//std::cout << "test 1 " << std::endl;
				if(time.Get() > 14 ){
					Sound sussurro = Sound ("audio/ghostly-whispers.wav");
					sussurro.Play(0);
					showBox = true;
					falas.SetText("U: OLHA O QUE FIZERAM COMIGO!");
					falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
					ultimoTempo = 14;
					showBox = true;
				}
				//std::cout << "test 2 " << std::endl;
				if(time.Get() > 18 ){
					//Sound sussurro = Sound ("audio/ghostly-whispers.wav");
					//sussurro.Play(0);
					falas.SetText("U: ACHA QUE CONSEGUE ME REPARAR?");
					falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
					ultimoTempo = 18;
					showBox = true;
				}
				if(time.Get() > 22 ){
					//Sound sussurro = Sound ("audio/ghostly-whispers.wav");
					//sussurro.Play(0);
					falas.SetText("U: DEVE TER ALGO NO QUARTO DA VELHA PRA AJUDAR");
					falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
					ultimoTempo = 22;
					showBox = true;
				}
				if(time.Get() > 26){
					//Sound sussurro = Sound ("audio/ghostly-whispers.wav");
					//sussurro.Play(0);
					falas.SetText(" ");
					falas.SetPos(0, Game::GetInstance().GetHeight()-POSY_FALA, true, false);
					ultimoTempo = 26;
					showBox = false;
				}
			}

			MessageDoor(dt);
			//TROCANDO DE COMODO. ENTRANDO NO CORREDOR PELA PRIMEIRA VEZ
	}

	if(time.Get() >= 4 && begin && fadeIn){
			UpdateVariable(dt, 80);
	}

	if(time.Get() >= 6){
		//PiscaPisca(dt, 6, 0.6);
	}

}

void Mission2::Render(){
	if(time.Get() < 4 && begin){
		blackSquare.Render(0, 0, 0);
		tx.Render(0,0);
		creepy.Render(0,0);
	} else if((time.Get() >= 4 && begin && fadeIn) || !bloqBlack){
		spFade.Render(0,0,0);
	}

	if(((MissionManager::missionManager->GetStage("StageState") &&
			MissionManager::missionManager->countStageState <= 1 && time.Get() > 4) ||
		((MissionManager::missionManager->GetStage("StageState") &&
			MissionManager::missionManager->countStageState > 1) ||
			MissionManager::missionManager->GetStage("HallState"))) &&
		!MissionManager::player->bloqHUD){
		if(showBox){
			falasBox.Render(falasBoxRect.x /*- Camera::pos.x*/, falasBoxRect.y /*- Camera::pos.y*/, 0);
			//printf("renderizando");
		}
		falas.Render(0,0);
	}
}

void Mission2::SetObjectStage(){
	Bear* bear = new Bear(800, 210);
	objectStage.emplace_back(bear);
	SceneObject* Bau = new SceneObject(300, 490,  "img/scene-bau-fechado.png", "img/scene-bau-fechado.png");
	objectStage.emplace_back(Bau);
	MovingObject* Box = new MovingObject(400, 500, "img/box.png");
	objectStage.emplace_back(Box);
	MovingObject* Cadeira = new MovingObject(730, 300, "img/scene-cadeira.png");
	objectStage.emplace_back(Cadeira);
}

void Mission2::SetObjectHall(){
	SceneObject* Window = new SceneObject(350, 70, "img/closedwindow.png", "img/openwindow.png");
	objectHall.emplace_back(Window);
}

void Mission2::SetObjectRoom(){

}
