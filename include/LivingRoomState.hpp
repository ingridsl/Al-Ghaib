#ifndef SRC_LIVINGROOMSTATE_HPP_
#define SRC_LIVINGROOMSTATE_HPP_

#include "State.hpp"
#include "Sprite.hpp"
#include "Geometry.hpp"
#include "TileSet.hpp"
#include "TileMap.hpp"
#include "Music.hpp"


class LivingRoomState : public State {
public:
	LivingRoomState(std::vector<std::unique_ptr<GameObject>> obj, bool inicial, int x, int y);
	~LivingRoomState();

	void Pause();
	void Resume();

	void LoadAssets();

	void Update(float dt);
	void Render();

	void SetInitialObjectArray();

private:
	Sprite background;
	Sprite bg;
	TileSet tileSetChao;
	TileSet tileSetParede;
	Music music;

public:
	TileMap tileMapChao;
	TileMap tileMapParede;
};

#endif /* SRC_LIVINGROOMSTATE_HPP_ */

