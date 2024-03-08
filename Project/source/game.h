#pragma once
#include "scene.h"
#include <random>

#define BLOCK_POSITION 80
#define DEFAULT_BLOCK 32
#define MAX_COIN 50
#define MAX_ENEMY 5
#define RANDOM_BLOCK 96

/// <summary>
/// ゲームマネージャークラス
/// </summary>
class Game:public Scene
{
private:
	D3DXVECTOR3 m_ArchiveRandomBlock[RANDOM_BLOCK]{};
	D3DXVECTOR3 m_BlockPosition[BLOCK_POSITION]{};

	class Audio* m_BGM{};
	class Audio* m_GameOver{};
	class Block* m_DefaultBlock[DEFAULT_BLOCK]{};
	class Block* m_RandomBlock[RANDOM_BLOCK]{};
	class BreakMap* m_BreakMap{};
	class Coin* m_Coin[MAX_COIN]{};
	class Enemy* m_Enemy[MAX_ENEMY]{};
	class Player* player{};
	class Wipe* m_Wipe{};
	class WarpBlock* m_WarpBlock{};

	int m_ArchiveNumber[BLOCK_POSITION] = {-1};
	bool m_IsGameOver;

public:
	void Init();
	void Uninit();
	void Update();

	void CreateCoin();
	void CreateFixedBlock();
	void CreateRandomBlock();
	void GameClear();
	void GameOver();
	void MapCreate();
	
};