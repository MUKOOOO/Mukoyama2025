#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "game.h"

#include "block.h"
#include "camera.h"
#include "coin.h"
#include "enemy.h"
#include "field.h"
#include "player.h"
#include "title.h"
#include "wall.h"
#include "warpBlock.h"

#include "breakMap.h"
#include "coinCount.h"
#include "wipe.h"


std::random_device rd;
std::mt19937 gen(rd());

int Random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

// 初期化処理
void Game::Init()
{
	e_sceneName = SCENE_NAME::STAGE;

	// モデル読み込み
	Coin().Load();
	Enemy().Load();
	Block().Load();
	WarpBlock().Load();

	// ブロックが配置される可能性のある位置を設定
	{
		int x, z;

		x = 0;
		z = 0;
		for (int i = 0; i < BLOCK_POSITION / 2; i++)
		{
			m_BlockPosition[i] = (D3DXVECTOR3(2.0f + x * 3.0f, 0.0f, z * 1.0f));

			x++;
			if (x % 4 == 0)
			{
				x = 0;
				z++;
				if (z % 3 - 2 == 0)
				{
					z++;
				}
			}
		}

		x = 0;
		z = 0;
		for (int i = 40; i < BLOCK_POSITION; i++)
		{
			m_BlockPosition[i] = (D3DXVECTOR3(x * 1.0f, 0.0f, 2.0f + z * 3.0f));

			z++;
			if (z % 4 == 0)
			{
				z = 0;
				x++;
				if (x % 3 - 2 == 0)
				{
					x++;
				}
			}
		}
	}
	
	// オブジェクトの登録
	AddGameObject<Camera>(0)->ChangeCameraType(Follow);

	AddGameObject<Field>(1)->SetPosition(D3DXVECTOR3(6.5f,0.0f,6.5f));

	AddGameObject<Wall>(1)->SetPosition(D3DXVECTOR3(6.5f, 2.0f, 13.5f));

	Wall* wallBack = AddGameObject<Wall>(1);
	wallBack->SetPosition(D3DXVECTOR3(6.5f, 2.0f, -0.5f));
	wallBack->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	Wall* wallLeft = AddGameObject<Wall>(1);
	wallLeft->SetPosition(D3DXVECTOR3(-0.5f, 2.0f, 6.5f));
	wallLeft->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI/2, 0.0f));

	Wall* wallRight = AddGameObject<Wall>(1);
	wallRight->SetPosition(D3DXVECTOR3(13.5f, 2.0f, 6.5f));
	wallRight->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f));

	player = AddGameObject<Player>(1);

	m_WarpBlock = AddGameObject<WarpBlock>(1);

	// UI
	CoinCount* coinCount = AddGameObject<CoinCount>(2);
	m_BreakMap = AddGameObject<BreakMap>(2);
	Wipe* wipe = AddGameObject<Wipe>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\sound\\Suno.wav");
	//m_BGM->Play(0.05f, true);

	// マップを生成
	MapCreate();
	
}

// 終了処理
void Game::Uninit()
{
	Scene::Uninit();

	WarpBlock().Unload();
	Block().Unload();
	Enemy().Unload();
	Coin().Unload();
}

// 更新処理
void Game::Update()
{
	Scene::Update();

	// プレイヤーの衝突判定
	switch (player->GetPlayerCollision())
	{
	case WARP_BLOCK:
		m_BreakMap->AddBreakMap(1);
		MapCreate();
		break;
	case ENEMY:
		GameOver();
		break;
	default:
		break;
	}

	if (Input::GetKeyTrigger('M'))
	{
		m_BreakMap->AddBreakMap(1);
		MapCreate();
	}
}

// コインの生成
void Game::CreateCoin()
{
	if (m_Coin[0] == NULL)
	{
		for (int i = 0; i < MAX_COIN; i++)
		{
			m_Coin[i] = AddGameObject<Coin>(1);
			m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));

			for (int x = 0; x < DEFAULT_BLOCK; x++)
			{
				while (m_Coin[i]->GetPosition() == m_DefaultBlock[x]->GetPosition())
				{
					m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));
				}
			}
			
		}
	}
	else
	{
		for (int i = 0; i < MAX_COIN; i++)
		{
			if (m_Coin[i] != NULL)m_Coin[i]->SetDestroy();
		}

		for (int i = 0; i < MAX_COIN; i++)
		{
			m_Coin[i] = AddGameObject<Coin>(1);
			m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));

			for (int x = 0; x < DEFAULT_BLOCK; x++)
			{
				while (m_Coin[i]->GetPosition() == m_DefaultBlock[x]->GetPosition())
				{
					m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));
				}
			}
		}
	}
}

// ブロック(固定)の生成
void Game::CreateFixedBlock()
{
	if (m_DefaultBlock[0] == NULL)
	{
		int x = 0;
		int y = 0;
		int z = 0;

		for (int i = 0; i < DEFAULT_BLOCK; i++)
		{
			m_DefaultBlock[i] = AddGameObject<Block>(1);
			m_DefaultBlock[i]->SetPosition(D3DXVECTOR3(2.0f + x * 3.0f, 0.0f + y * 1.0f, 2.0f + z * 3.0f));

			x++;

			if (x / 4 == 1)
			{
				x = 0;
				z++;
			}

			if (z / 4 == 1)
			{
				z = 0;
				y++;
			}
		}
	}
}

// ブロック(ランダム)の生成
void Game::CreateRandomBlock()
{
	if (m_RandomBlock[0] == NULL)
	{
		for (int i = 0; i < RANDOM_BLOCK; i+=2)
		{
			m_RandomBlock[i] = AddGameObject<Block>(1);
			m_RandomBlock[i]->SetPosition(D3DXVECTOR3(m_BlockPosition[Random(0, 79)]));

			for (int x = 0; x < RANDOM_BLOCK; x++)
			{
				if (x == i)break;

				while (m_RandomBlock[i]->GetPosition() == m_RandomBlock[x]->GetPosition())
				{
					m_RandomBlock[i]->SetPosition(D3DXVECTOR3(m_BlockPosition[Random(0, 79)]));
				}
			}
			m_RandomBlock[i+1] = AddGameObject<Block>(1);
			m_RandomBlock[i+1]->SetPosition(D3DXVECTOR3(m_RandomBlock[i]->GetPosition().x,1.0f, m_RandomBlock[i]->GetPosition().z));
		}
	}
	else
	{
		for (int i = 0; i < RANDOM_BLOCK; i += 2)
		{
			m_RandomBlock[i]->SetPosition(D3DXVECTOR3(m_BlockPosition[Random(0, 79)]));

			for (int x = 0; x < RANDOM_BLOCK; x++)
			{
				if (x == i)break;

				while (m_RandomBlock[i]->GetPosition() == m_RandomBlock[x]->GetPosition())
				{
					m_RandomBlock[i]->SetPosition(D3DXVECTOR3(m_BlockPosition[Random(0, 79)]));
				}
			}
			m_RandomBlock[i + 1]->SetPosition(D3DXVECTOR3(m_RandomBlock[i]->GetPosition().x, 1.0f, m_RandomBlock[i]->GetPosition().z));
		}
	}
}

void Game::GameClear()
{

}

void Game::GameOver()
{
	Manager::SetScene<Title>();
}

// マップを生成
void Game::MapCreate()
{
	if (m_BreakMap->GetBreakMap() < 5)
	{
		// ブロック(固定)
		CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// エネミー パックンフラワーのような敵
		/*if (m_Enemy[0] != NULL)
		{
			for (int i = 0; i < MAX_ENEMY; i++)
			{
				if (m_Enemy[i] != NULL)m_Enemy[i]->SetDestroy();
			}
		}

		for (int i = 0; i < m_BreakMap->GetBreakMap() + 1; i++)
		{
			m_Enemy[i] = AddGameObject<Enemy>(1);
			m_Enemy[i]->SetPosition(D3DXVECTOR3(Random(0, 14), 0.0f, Random(0, 14)));
		}*/
		
		// コイン
		CreateCoin();
		
		m_WarpBlock->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 12.5f));
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.5f));
	}
	else if (m_BreakMap->GetBreakMap() == 5) //レストフロア
	{
		// ブロック(固定)の削除
		for (int i = 0; i < DEFAULT_BLOCK; i++)
		{
			m_DefaultBlock[i]->SetDestroy();
			m_DefaultBlock[i] = NULL;
		}

		// ブロック(ランダム)の削除
		for (int i = 0; i < RANDOM_BLOCK; i++)
		{
			m_RandomBlock[i]->SetDestroy();
			m_RandomBlock[i] = NULL;
		}

		// コインの削除
		for (int i = 0; i < MAX_COIN; i++)
		{
			if (m_Coin[i] != NULL)m_Coin[i]->SetDestroy();
			m_Coin[i] = NULL;
		}

		m_WarpBlock->SetPosition(D3DXVECTOR3(6.5f, 0.5f, 11.5f));
		player->SetPosition(D3DXVECTOR3(6.5f, 0.0f, 1.5f));
	}
	else if (m_BreakMap->GetBreakMap() < 10)
	{
		// ブロック(固定)
		CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// エネミー カメックみたいな敵
		
		// コイン
		CreateCoin();

		m_WarpBlock->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 12.5f));
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.5f));
	}
	else if (m_BreakMap->GetBreakMap() < 15)
	{
		// ブロック(固定)
		CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// エネミー ファイアパックンみたいな敵

		// コイン
		CreateCoin();

		m_WarpBlock->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 12.5f));
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.5f));
	}
	else if (m_BreakMap->GetBreakMap() == 15) //レストフロア
	{
		// ブロック(固定)の削除
		for (int i = 0; i < DEFAULT_BLOCK; i++)
		{
			m_DefaultBlock[i]->SetDestroy();
			m_DefaultBlock[i] = NULL;
		}

		// ブロック(ランダム)の削除
		for (int i = 0; i < RANDOM_BLOCK; i++)
		{
			m_RandomBlock[i]->SetDestroy();
			m_RandomBlock[i] = NULL;
		}

		// コインの削除
		for (int i = 0; i < MAX_COIN; i++)
		{
			if (m_Coin[i] != NULL)m_Coin[i]->SetDestroy();
			m_Coin[i] = NULL;
		}

		m_WarpBlock->SetPosition(D3DXVECTOR3(6.5f, 0.5f, 11.5f));
		player->SetPosition(D3DXVECTOR3(6.5f, 0.0f, 1.5f));
	}
	else if (m_BreakMap->GetBreakMap() < 20)
	{
		// ブロック(固定)
		CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// エネミー テレサみたいな敵

		// コイン
		CreateCoin();

		m_WarpBlock->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 12.5f));
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.5f));
	}
	else if (m_BreakMap->GetBreakMap() < 25)
	{
		// ブロック(固定)
		CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// エネミー 今までに登場した敵を組み合わせる

		// コイン
		CreateCoin();

		m_WarpBlock->SetPosition(D3DXVECTOR3(0.5f, 0.5f, 12.5f));
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.5f));
	}
	else if (m_BreakMap->GetBreakMap() == 25)
	{
		// ブロック(固定)の削除
		for (int i = 0; i < DEFAULT_BLOCK; i++)
		{
			m_DefaultBlock[i]->SetDestroy();
			m_DefaultBlock[i] = NULL;
		}

		// ブロック(ランダム)の削除
		for (int i = 0; i < RANDOM_BLOCK; i++)
		{
			m_RandomBlock[i]->SetDestroy();
			m_RandomBlock[i] = NULL;
		}

		// コインの削除
		for (int i = 0; i < MAX_COIN; i++)
		{
			if (m_Coin[i] != NULL)m_Coin[i]->SetDestroy();
			m_Coin[i] = NULL;
		}

		m_WarpBlock->SetPosition(D3DXVECTOR3(6.5f, 0.5f, 11.5f));
		player->SetPosition(D3DXVECTOR3(6.5f, 0.0f, 1.5f));
	}
}