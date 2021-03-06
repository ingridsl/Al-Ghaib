#ifndef SRC_HALLSTATE_HPP_
#define SRC_HALLSTATE_HPP_

#include "State.hpp"
#include "Sprite.hpp"
#include "Geometry.hpp"
#include "TileSet.hpp"
#include "TileMap.hpp"
#include "Music.hpp"


class HallState : public State {
public:
	HallState(std::vector<std::unique_ptr<GameObject>> obj, bool inicial, int x, int y);
	~HallState();

	void Pause();
	void Resume();

	void LoadAssets();

	void Update(float dt);
	void Render();

	void SetInitialObjectArray();

private:
	Sprite background;
	Sprite bg;
	TileSet tileSet;

	Music music;

public:
	TileMap tileMap;
};

#endif /* SRC_HALLSTATE_HPP_ */
