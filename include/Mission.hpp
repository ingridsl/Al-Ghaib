#ifndef SRC_MISSION_HPP_
#define SRC_MISSION_HPP_

#define OPAQUE SDL_ALPHA_OPAQUE
#define TRANSPARENT SDL_ALPHA_TRANSPARENT

#ifdef __APPLE__
	#include <SDL2.h>
	#include <SDL_image/SDL_image.h>
	#include <SDL_mixer/SDL_mixer.h>
	#include <SDL_ttf/SDL_ttf.h>
#elif __linux__
    #include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>
#elif _WIN32
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_mixer.h"
	#include "SDL_ttf.h"
#endif

#include <vector>
#include <memory>
#include <string>

#include "GameObject.hpp"
#include "Text.hpp"
#include "Timer.hpp"
#include "Sprite.hpp"
#include "InputManager.hpp"
#include "MissionManager.hpp"
#include "Sound.hpp"
#include "Geometry.hpp"

#define POSY_FALA 70

class Mission {
public:
	Mission();
	Mission(Mission&&) = default;
	Mission& operator=(Mission&&) = default;
	virtual ~Mission();

	std::string GetInitialState();
	int GetInitialX();
	int GetInitialY();
	std::vector<std::unique_ptr<GameObject>> GetObjectStage();
	std::vector<std::unique_ptr<GameObject>> GetObjectHall();
	std::vector<std::unique_ptr<GameObject>> GetObjectLivingRoom();
	std::vector<std::unique_ptr<GameObject>> GetObjectMomRoom();

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	bool PopRequested();
	bool QuitRequested();

	void MessageDoor(float dt);
	void UpdateVariable(float dt, float speed, bool turnOpaque = false);
	void SetPiscaPisca(int max, float time);
	void PiscaPisca(float dt);
	void ImageProfileBox (int option);


	bool bronca;
	int first;

protected:
	bool popRequested, quitRequested;

	virtual void SetObjectStage() = 0;
	virtual void SetObjectHall() = 0;
	virtual void SetObjectLivingRoom() = 0;
	virtual void SetObjectMomRoom() = 0;

	std::vector<std::unique_ptr<GameObject>> objectStage, objectHall, objectLivingRoom, objectMomRoom;
	std::string initialState;
	int initialX, initialY;

	Text tx;
	Text creepy;
	Text falas;
	Timer cooldown;
	Timer time;
	Timer timerPisca;
	Sprite blackSquare;
	float ultimoTempo;
	bool flagTimer;
	bool trancada;
	int state;
	bool doorSound;

	Sprite spFade;
	int alpha;
	float alphaAux;
	bool bloqBlack;
	int contPisca;
	bool fadeIn;

	Sprite falasBox;
	bool showBox;
	Rect falasBoxRect;

	bool gameOver;
	int maxPisca;
	float timePisca;

	Sprite profileBox;
	int profileBoxX, profileBoxY;
};

#endif /* SRC_MISSION_HPP_ */
