#include "TitleState.hpp"
#include "Game.hpp"
#include "InputManager.hpp"
#include "MissionManager.hpp"

Music TitleState::intro, TitleState::music;
bool TitleState::stopMusic;

TitleState::TitleState() : menu(50, 150) {
	//SDL_Color auxcolor = SDL_Color();
	//auxcolor.r = 255;
	//auxcolor.g = 10;
	//auxcolor.b = 10;

	flagTimer = true; stopMusic = false; controlPop = false;
	time = Timer();
	bg = Sprite("img/menu/sprite-title.png", 5, 0.7);
	bg.SetScaleX(1.5); bg.SetScaleY(1.5);
	control = Sprite("img/menu/controles.png");
	shift = false;

	//tx = Text("font/Xposed.ttf", 80, Text::TextStyle::BLENDED, "AL-GHAIB", auxcolor, 0, 0);
	//tx.SetPos(0, 0, true, true);
	intro = Music("audio/menu-intro.wav");
	music = Music("audio/menu-loop.wav");
	intro.Play(1);
	Mix_HookMusicFinished(&TitleState::LinkMusic);
}

void TitleState::Update(float dt){
	InputManager instance = InputManager::GetInstance();

	if(bg.GetCurrentFrame() == 5){
		shift = true;
	} else if(bg.GetCurrentFrame() == 1){
		shift = false;
	}
	bg.Update(dt, 0, shift);
	if(controlPop){
		if(instance.KeyPress(ESCAPE_KEY) || instance.KeyPress(ENTER_KEY)) controlPop = false;
	} else{

		if(instance.KeyPress(ESCAPE_KEY)) quitRequested = true;
		else quitRequested = instance.QuitRequested();

		menu.Update(dt);
		if(menu.IsSelected()){
			menu.SetSelected(false);
			switch(menu.GetOption()){
				case MENU_START:
					Pause();
					Game::GetInstance().GetMissionManager().ChangeMission(1);
					break;
				case MENU_CONTINUE:
					Pause();
					MissionManager::missionManager->LoadMission();
					break;
				case MENU_OPTIONS:
					controlPop = true;
					break;
				case MENU_EXIT:
					Pause();
					quitRequested = true;
					break;
			}
		}

	}
}

void TitleState::Render(){
	bg.Render(0,0,0);
	//tx.Render(0,0);
	menu.Render();
	if(controlPop) control.Render(50,50,0);
}

void TitleState::Pause(){
	stopMusic = true;
	intro.Stop();
	music.Stop();
}

void TitleState::Resume(){
	Game::GetInstance().GetMissionManager().DeleteStates();
	stopMusic = false;
	intro.Play(1);
}

void TitleState::LinkMusic(){
	if(!stopMusic){
		music.Play(-1);
	}
}
