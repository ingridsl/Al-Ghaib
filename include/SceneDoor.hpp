#ifndef SRC_SCENEDOOR_HPP_
#define SRC_SCENEDOOR_HPP_

#include "SceneObject.hpp"
#include "Text.hpp"
#include <iostream>

#define ABRE 0
//#define FECHA 2

class SceneDoor : public SceneObject {
public:
	SceneDoor(float x, float y, std::string dest,bool locked = true,
			std::string img = "img/scene-door-closed.png", std::string img2 = "img/scene-door-opened.png");

	bool NotifyCollision(GameObject& other);
	bool Is(std::string type);
	bool ReceiveAction(InventoryObject* other);
	bool GetChangeState();
	void SetChangeState(bool change);
	std::string GetDest();
	//bool Tried();
	static int ValorPassar;
	static int count;
	bool changeState;

private:
	int contador;
	bool lock;
	std::string dest;

	//bool tried;
};

#endif /* SRC_SCENEDOOR_HPP_ */
