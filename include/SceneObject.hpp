#ifndef SRC_SCENEOBJECT_HPP_
#define SRC_SCENEOBJECT_HPP_

#include <string>

#include "GameObject.hpp"
#include "Sprite.hpp"
#include "InventoryObject.hpp"
#include "Camera.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include "MovingObject.hpp"

#define  OFFSET_PISO 10

class SceneObject : public GameObject {
public:
	SceneObject(float x, float y, std::string img, std::string img2);

	void Update(float dt);
	void Render();
	bool IsEstado();
	bool IsDead();
	bool NotifyCollision(GameObject& other);
	bool Is(std::string type);

protected:
	Sprite sp;
	bool estado;
	std::string change1, change2;
};

#endif  /* SCENEOBJECT_H_ */
