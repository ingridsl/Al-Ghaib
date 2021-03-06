#ifndef INCLUDE_Player_HPP_
#define INCLUDE_Player_HPP_

#include "InventoryObject.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Game.hpp"
#include "Timer.hpp"
#include "InputManager.hpp"
#include "Timer.hpp"

#include <vector>
#include <memory>
#include <string>

class Player : public GameObject{
public:
	enum InvBox{SUL, NORTE, LESTE, OESTE};

	Player(float x, float y, int oldInHand = -1, std::vector<std::string> oldInventory = std::vector<std::string>());
	~Player();

	void Update(float dt);
	void Render();
	bool IsDead();
	bool NotifyCollision(GameObject& other);
	bool Is(std::string type);

	void SetPosition(int x, int y);
	void SetDirecao(int dir);
	void SetBlocked(bool b);
	bool CollidingPlayer(Rect box, float offset);
	void Running(InputManager InputInstance);

	void SetMovementLimits(Rect limits);
	void AddWallLimits(Rect limits);
	void ResetWallLimits();
	void ChangeHiddenState();
	void ChangeAboveObject();

	bool GetAboveObject();
	int GetDirecao();
	Vec2 GetSpeed();
	void SetDoor(bool value);
	bool GetDoor();
	void SetBloqHUD(bool value);
	bool GetBloqHUD();
	void SetBloqInv(bool value);
	bool GetBloqInv();

	void AddRuido(float add);
	float GetRuido();

	void AddInventory(std::string obj);
	std::vector<std::shared_ptr<InventoryObject>> GetInventory();
	std::vector<std::string> GetStringInventory();
	bool GetShowingInventory();
	bool HaveObject(std::string obj);
	std::shared_ptr<InventoryObject> GetInHand();
	int GetInHandIndex();
	void DeleteInventory();

	void RenderInventory();
	void RenderInHand();
	void RenderNoise();

	//Vec2 previousPos;
	Rect limits;
	bool hidden, climbing, climbingDown, drogado;
	int dirDown;
	float centerMX, centerMY;
	std::string lastPicked;
	std::vector<Rect> wallLimits;

private:
	Timer timeRuido, timeCooldown, timeSound, timePicked;
	Sprite spKinder, spKinderRun, spAnimKinder, spKinderPush, spKinderClimbing, spKinderDown;
	Sprite spInventory, spInventorybox, spInventoryboxSelected;
	Sprite spNoise, spPicked;

	Vec2 speed;
	bool running, direcaoShift;

	int inHandIndex, inventoryIndex;
	bool showingInventory, showPicked;
	std::vector<std::shared_ptr<InventoryObject>> inventory;

	InvBox direcao;
	InputManager InputInstance;

	float ruido;
	bool door, animShowing, aboveObject;
	bool blocked, bloqHUD, bloqInv;
};

#endif /* INCLUDE_Player_HPP_ */
