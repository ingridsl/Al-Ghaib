#ifndef SRC_MISSION6_HPP_
#define SRC_MISSION6_HPP_

#include "Mission.hpp"
#include "InputManager.hpp"
#include "Text.hpp"
#include "Timer.hpp"
#include "Sprite.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bear.hpp"
#include "Music.hpp"
#include "Sound.hpp"
#include "Cat.hpp"

class Mission6 : public Mission {
public:
	Mission6();
	~Mission6();

	void Pause();
	void Resume();

	void Update(float dt);
	void Render();
private:
	void SetObjectStage();
	void SetObjectHall();
	void SetObjectLivingRoom();
	void SetObjectMomRoom();

	static Music music;
	bool paradoUrso, paradoGato;
	bool played, endMission;
	int countMom, countTitle;
};

#endif /* SRC_MISSION6_HPP_ */
