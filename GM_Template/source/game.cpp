#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "game.h"

#include "camera.h"
#include "coin.h"
#include "enemy.h"
#include "field.h"
#include "player.h"
#include "simple3d.h"
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
	Simple3d().Load();
	WarpBlock().Load();

	// ブロックが配置される可能性のある位置を設定
	for (int horizontal = 0; horizontal < 4; horizontal++)
	{
		for (int vertically = 0; vertically < 5; vertically++)
		{
			m_BlockPosition[horizontal * 10 + vertically * 2] = D3DXVECTOR3(horizontal * 3 + 2, 0.0f, vertically * 3);
			m_BlockPosition[horizontal * 10 + vertically * 2 + 1] = D3DXVECTOR3(horizontal * 3 + 2, 0.0f, vertically * 3 + 1);
		}
	}

	for (int horizontal = 0; horizontal < 4; horizontal++)
	{
		for (int vertically = 0; vertically < 5; vertically++)
		{
			m_BlockPosition[40 + horizontal * 10 + vertically * 2] = D3DXVECTOR3(vertically * 3, 0.0f, horizontal * 3 + 2);
			m_BlockPosition[40 + horizontal * 10 + vertically * 2 + 1] = D3DXVECTOR3(vertically * 3 + 1, 0.0f, horizontal * 3 + 2);
		}
	}

	int x, z;
	x = 2.0f;
	z = 2.0f;
	for (int i = 0; i < 80; i++)
	{
		m_BlockPosition[i] = D3DXVECTOR3(x * 2, 0.0f, z * 3);
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

	AddGameObject<WarpBlock>(1);

	// ブロックの生成(ランダム)
	//for (int i = 0; i <= RANDOM_BLOCK / 2; i++)
	//{
	//	m_RandomBlock[i*2] = AddGameObject<Simple3d>(1);
	//	m_RandomBlock[i*2+1] = AddGameObject<Simple3d>(1);
	//	m_ArchiveNumber[i] = Random(0, 79);
	//	D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];

	//	// ブロックの位置の重複を無くす
	//	for (int q = 0; q < RANDOM_BLOCK / 2; q++)
	//	{
	//		if (m_ArchiveNumber[q] = -1)
	//		{
	//			break;
	//		}

	//		while (m_ArchiveNumber[i] == m_ArchiveNumber[q])
	//		{
	//			m_ArchiveNumber[i] = Random(0, 79);
	//			D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];
	//		}
	//	}
	//	
	//	m_RandomBlock[i*2]->SetPosition(pos);
	//	m_RandomBlock[i*2+1]->SetPosition(D3DXVECTOR3(pos.x,1.0f,pos.z));
	//}

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
	Simple3d().Unload();
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
		player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.0f));
		m_BreakMap->AddBreakMap(1);
		MapCreate();
		break;
	case ENEMY:
		GameOver();
		Manager::SetScene<Title>();
		break;
	default:
		break;
	}
}

/// <summary>
/// ブロック(固定)の生成
/// </summary>
void Game::CreateFixedBlock()
{
	int x = 0;
	int y = 0;
	int z = 0;

	for (int i = 0; i < DEFAULT_BLOCK;i++)
	{
		m_DefaultBlock[i] = AddGameObject<Simple3d>(1);
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

/// <summary>
/// ブロック(ランダム)の生成
/// </summary>
void Game::CreateRandomBlock()
{
	int x, z;

	x = 0;
	z = 0;

	for (int i = 0; i < BLOCK_POSITION/2; i++)
	{
		m_RandomBlock[i] = AddGameObject<Simple3d>(1);
		m_RandomBlock[i]->SetPosition(D3DXVECTOR3(2.0f + x * 3.0f, 0.0f, z * 1.0f));

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
		m_RandomBlock[i] = AddGameObject<Simple3d>(1);
		m_RandomBlock[i]->SetPosition(D3DXVECTOR3(x * 1.0f, 0.0f, 2.0f + z * 3.0f));

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

	//int y = 0;

	//if (m_RandomBlock[0] == NULL)
	//{
	//	for (int i = 0; i < RANDOM_BLOCK; i++)
	//	{
	//		m_RandomBlock[i] = AddGameObject<Simple3d>(1);
	//		if(m_RandomBlock[i] != NULL)m_RandomBlock[i]->SetPosition(D3DXVECTOR3(Random(0, 14), 0.0f + y * 1.0f, Random(0, 14)));

	//		if(y / 1 == 1) y = 0;
	//	}

	//	for (int i = 0; i <= RANDOM_BLOCK / 2; i++)
	//	{
	//		m_RandomBlock[i * 2] = AddGameObject<Simple3d>(1);
	//		m_RandomBlock[i * 2 + 1] = AddGameObject<Simple3d>(1);
	//		m_ArchiveNumber[i] = Random(0, RANDOM_BLOCK-1);
	//		D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];

	//		// ブロックの位置の重複を無くす
	//		for (int q = 0; q < RANDOM_BLOCK / 2; q++)
	//		{
	//			if (m_ArchiveNumber[q] = -1)
	//			{
	//				break;
	//			}

	//			while (m_ArchiveNumber[i] == m_ArchiveNumber[q])
	//			{
	//				m_ArchiveNumber[i] = Random(0, 79);
	//				D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];
	//			}
	//		}

	//		if (m_RandomBlock[i*2] != NULL)m_RandomBlock[i * 2]->SetPosition(D3DXVECTOR3(pos.x, 0.0f, pos.z));
	//		if (m_RandomBlock[i*2+1] != NULL)m_RandomBlock[i * 2 + 1]->SetPosition(D3DXVECTOR3(pos.x, 1.0f, pos.z));
	//	}
	//}
	//else
	//{

	//	for (int i = 0; i < RANDOM_BLOCK / 2; i++)
	//	{
	//		m_ArchiveNumber[i] = Random(0, 79);
	//		D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];

	//		for (int q = 0; q <= RANDOM_BLOCK / 2; q++)
	//		{
	//			if (m_ArchiveNumber[q] = -1)
	//			{
	//				break;
	//			}

	//			while (m_ArchiveNumber[i] == m_ArchiveNumber[q])
	//			{
	//				m_ArchiveNumber[i] = Random(0, 79);
	//				D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];
	//			}
	//		}

	//		m_RandomBlock[i * 2]->SetPosition(pos);
	//		m_RandomBlock[i * 2 + 1]->SetPosition(D3DXVECTOR3(pos.x, 1.0f, pos.z));
	//	}
	//}
}

void Game::GameClear()
{

}

void Game::GameOver()
{

}

/// <summary>
/// マップを生成する関数
/// </summary>
void Game::MapCreate()
{
	if (m_BreakMap->GetBreakMap() < 5)
	{
		// ブロック(固定)
		//CreateFixedBlock();

		// ブロック(ランダム)
		CreateRandomBlock();

		// 敵
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
		
		// コインの生成
		/*if (m_Coin[0] == NULL)
		{
			for (int i = 0; i < 100; i++)
			{
				m_Coin[i] = AddGameObject<Coin>(1);
				m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));
			}
		}
		else
		{
			for (int i = 0; i < 100; i++)
			{
				if(m_Coin[i] != NULL)m_Coin[i]->SetDestroy();
			}

			for (int i = 0; i < 100; i++)
			{
				m_Coin[i] = AddGameObject<Coin>(1);
				m_Coin[i]->SetPosition(D3DXVECTOR3(Random(1, 13), 0.0f, Random(1, 13)));
			}
		}*/
		
	}
	else if (m_BreakMap->GetBreakMap() == 5)
	{
		/*for (int i = 0; i < DEFAULT_BLOCK; i++)
		{
			m_DefaultBlock[i]->SetDestroy();
		}

		for (int i = 0; i < RANDOM_BLOCK; i++)
		{
			m_RandomBlock[i]->SetDestroy();
		}*/
	}
	else if (m_BreakMap->GetBreakMap() < 9)
	{

	}
	else if (m_BreakMap->GetBreakMap() < 14)
	{

	}
	else if (m_BreakMap->GetBreakMap() < 19)
	{

	}
	else if (m_BreakMap->GetBreakMap() < 24)
	{

	}
	else if (m_BreakMap->GetBreakMap() == 25)
	{

	}
}