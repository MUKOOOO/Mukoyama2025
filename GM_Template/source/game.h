#pragma once
#include "scene.h"
#include <random>

#define DEFAULT_BLOCK 32
#define RANDOM_BLOCK 32

class Game:public Scene
{
private:
	D3DXVECTOR3 m_ArchiveRandomBlock[RANDOM_BLOCK]{};

	class Simple3d* m_DefaultBlock[DEFAULT_BLOCK]{};
	class Simple3d* m_RandomBlock[RANDOM_BLOCK]{};

	class Player* player{};
	class Score* score{};

	int m_BreakMap;
	int m_Frame;

public:
	void Init();
	void Uninit();
	void Update();

	void GameClear();
	void ChangeRandomBlock();
	int GetFrame() { return m_Frame; }
	
};