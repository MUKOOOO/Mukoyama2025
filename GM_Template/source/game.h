#pragma once
#include "scene.h"
#include <random>

#define DEFAULT_BLOCK 32
#define RANDOM_BLOCK 96
#define BLOCK_POSITION 80

class Game:public Scene
{
private:
	D3DXVECTOR3 m_ArchiveRandomBlock[RANDOM_BLOCK]{};
	D3DXVECTOR3 m_BlockPosition[BLOCK_POSITION]{};

	class Simple3d* m_DefaultBlock[DEFAULT_BLOCK]{};
	class Simple3d* m_RandomBlock[RANDOM_BLOCK]{};

	class Audio* m_BGM{};
	class Player* player{};
	class Score* score{};

	int m_ArchiveNumber[BLOCK_POSITION] = {-1};
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