#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "camera.h"
#include "field.h"
#include "player.h"
#include "simple3d.h"
#include "goal.h"
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
	Simple3d().Load();
	Goal().Load();

	AddGameObject<Camera>(0)->ChangeCameraType(Follow);

	AddGameObject<Field>(1);

	AddGameObject<Player>(1);

	// 前後左右に柵を配置
	Simple3d* WallEast[14];
	Simple3d* WallWest[14];
	Simple3d* WallSouth[14];
	Simple3d* WallNorth[14];
	
	for (int i = 0; i < 14; i++)
	{
		WallEast[i] = AddGameObject<Simple3d>(1);
		WallEast[i]->SetPosition(D3DXVECTOR3(28.0f, 0.0f, (float)i * 2.0f));

		WallWest[i] = AddGameObject<Simple3d>(1);
		WallWest[i]->SetPosition(D3DXVECTOR3(-2.0f, 0.0f, (float)i * 2.0f));

		WallSouth[i] = AddGameObject<Simple3d>(1);
		WallSouth[i]->SetPosition(D3DXVECTOR3((float)i * 2.0f, 0.0f, 28.0f));

		WallNorth[i] = AddGameObject<Simple3d>(1);
		WallNorth[i]->SetPosition(D3DXVECTOR3((float)i * 2.0f, 0.0f, -2.0f));
	}

	// ブロックを配置(固定)----------------------------------------------------------------
	for (int x = 0; x < 4; x++)
	{
		//4.0,10.0,16.0,22.0
		block[x*2] = AddGameObject<Simple3d>(1);
		block[x*2]->SetPosition(D3DXVECTOR3(4.0f + x * 6,0.0f,4.0f));
		block[x*2+1] = AddGameObject<Simple3d>(1);
		block[x*2+1]->SetPosition(D3DXVECTOR3(4.0f + x * 6, 2.0f, 4.0f));
	}

	// ブロックを配置(ランダム)----------------------------------------------------------------
	/*for (int i = 0; i < 10; i++)
	{
		block[i] = AddGameObject<Simple3d>(1);
		D3DXVECTOR3 pos;
		block[i]->SetPosition(D3DXVECTOR3(pos.x = Random(2, 28), 0.0f, pos.z = Random(2, 28)));
	}*/

	AddGameObject<Goal>(1)->SetPosition(D3DXVECTOR3(0.5f,0.5f,25.5f));

	m_Frame = 0;
}

void Game::Uninit()
{
	Scene::Uninit();

	Goal().Unload();
	Simple3d().Unload();
}

void Game::Update()
{
	Scene::Update();

	m_Frame++;

	if (m_Frame >= 12)
	{
		m_Frame = 0;
	}
}