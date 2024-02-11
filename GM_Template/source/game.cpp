#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "game.h"

#include "camera.h"
#include "field.h"
#include "player.h"
#include "sky.h"
#include "simple3d.h"
#include "warpBlock.h"

#include "breakMap.h"
#include "hitEffect.h"


std::random_device rd;
std::mt19937 gen(rd());

int Random(int low, int high)
{
	std::uniform_int_distribution<> dist(low, high);
	return dist(gen);
}

void Game::Init()
{
	e_sceneName = SCENE_NAME::STAGE;

	Simple3d().Load();
	WarpBlock().Load();

	//ブロックが配置される可能性のある位置を設定
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

	AddGameObject<Camera>(0)->ChangeCameraType(Follow);
	AddGameObject<Sky>(1);
	AddGameObject<Field>(1)->SetPosition(D3DXVECTOR3(6.5f,0.0f,6.5f));

	// 前後左右に柵を配置----------------------------------------------------------------
	Simple3d* WallEast[14];
	Simple3d* WallWest[14];
	Simple3d* WallSouth[14];
	Simple3d* WallNorth[14];
	
	for (int i = 0; i < 14; i++)
	{
		WallEast[i] = AddGameObject<Simple3d>(1);
		WallEast[i]->SetPosition(D3DXVECTOR3(14.0f, 0.0f, (float)i * 1.0f));

		WallWest[i] = AddGameObject<Simple3d>(1);
		WallWest[i]->SetPosition(D3DXVECTOR3(-1.0f, 0.0f, (float)i * 1.0f));

		WallSouth[i] = AddGameObject<Simple3d>(1);
		WallSouth[i]->SetPosition(D3DXVECTOR3((float)i, 0.0f, 14.0f));

		WallNorth[i] = AddGameObject<Simple3d>(1);
		WallNorth[i]->SetPosition(D3DXVECTOR3((float)i, 0.0f, -1.0f));
	}

	// ブロックを配置(固定)----------------------------------------------------------------
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			//2.0,5.0,8.0,11.0
			m_DefaultBlock[x + y * 2] = AddGameObject<Simple3d>(1);
			m_DefaultBlock[x + y * 2]->SetPosition(D3DXVECTOR3(2.0f + x * 3, 0.0f, 2.0f + y * 3));
			m_DefaultBlock[x + y * 2 + 1] = AddGameObject<Simple3d>(1);
			m_DefaultBlock[x + y * 2 + 1]->SetPosition(D3DXVECTOR3(2.0f + x * 3, 1.0f, 2.0f + y * 3));
		}
	}

	m_RandomBlock[0] = AddGameObject<Simple3d>(1);
	D3DXVECTOR3 pos = m_BlockPosition[Random(0, 79)];
	m_RandomBlock[0]->SetPosition(pos);

	// ブロックを配置(ランダム)----------------------------------------------------------------
	for (int i = 0; i <= RANDOM_BLOCK / 2; i++)
	{
		m_RandomBlock[i*2] = AddGameObject<Simple3d>(1);
		m_RandomBlock[i*2+1] = AddGameObject<Simple3d>(1);
		m_ArchiveNumber[i] = Random(0, 79);
		D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];

		// ブロックの位置の重複を無くす
		for (int q = 0; q < RANDOM_BLOCK / 2; q++)
		{
			if (m_ArchiveNumber[q] = -1)
			{
				break;
			}

			while (m_ArchiveNumber[i] == m_ArchiveNumber[q])
			{
				m_ArchiveNumber[i] = Random(0, 79);
				D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];
			}
		}
		
		m_RandomBlock[i*2]->SetPosition(pos);
		m_RandomBlock[i*2+1]->SetPosition(D3DXVECTOR3(pos.x,1.0f,pos.z));
	}

	AddGameObject<WarpBlock>(1);

	player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.0f));

	m_BreakMap = AddGameObject<BreakMap>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\sound\\Suno.wav");
	m_BGM->Play(0.05f, true);
}

void Game::Uninit()
{
	Scene::Uninit();

	WarpBlock().Unload();
	Simple3d().Unload();
}

void Game::Update()
{
	Scene::Update();

	// ゴールブロックと衝突した場合の処理
	if (player->GetPlayerCollision() == WARP_BLOCK)
	{
		player->SetPlayerCollision(NONE);

		// 突破したマップの数が指定の数と異なる場合ダンジョンを再生成
		if (m_BreakMap->GetBreakMap() == 3)
		{
			GameClear();
		}
		else
		{
			player->SetPosition(D3DXVECTOR3(13.0f, 0.0f, 0.0f));
			ChangeRandomBlock();
			m_BreakMap->AddBreakMap(1);
		}			
	}
}

/// <summary>
/// ブロックの配置を変更する関数
/// </summary>
void Game::ChangeRandomBlock()
{
	for (int i = 0; i < RANDOM_BLOCK / 2; i++)
	{
		m_ArchiveNumber[i] = Random(0, 79);
		D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];

		for (int q = 0; q <= RANDOM_BLOCK / 2; q++)
		{
			if (m_ArchiveNumber[q] = -1)
			{
				break;
			}

			while (m_ArchiveNumber[i] == m_ArchiveNumber[q])
			{
				m_ArchiveNumber[i] = Random(0, 79);
				D3DXVECTOR3 pos = m_BlockPosition[m_ArchiveNumber[i]];
			}
		}

		m_RandomBlock[i * 2]->SetPosition(pos);
		m_RandomBlock[i * 2 + 1]->SetPosition(D3DXVECTOR3(pos.x, 1.0f, pos.z));
	}
}

void Game::GameClear()
{

}