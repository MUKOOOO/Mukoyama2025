#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "title.h"
#include "perlinNoise.h"
#include "input.h"
#include "camera.h"
#include "sky.h"
#include "grassBlock.h"
#include "simple3d.h"
#include "fade.h"
#include "wipe.h"
#include "game.h"

std::random_device rd2;
std::mt19937 gen2(rd2());

int Random2(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen2);
}

float Title::m_bump{};
float Title::m_maxHeight{};

void Title::Init()
{
    e_sceneName = SCENE_NAME::TITLE;

    // 3Dモデルの読み込み
    GrassBlock().Load();
    Simple3d().Load();

    // オブジェクトの配置
	camera = AddGameObject<Camera>(0);
	camera->SetPosition(D3DXVECTOR3(12.0f, 5.05f, -6.0f));
	camera->SetTarget(D3DXVECTOR3(12.0f, 5.0f, 0.0f));

	AddGameObject<Sky>(1);

    // パーリンノイズを使用した地形生成
    m_bump = 10.0f;
    m_maxHeight = 4.5;
    m_seedX = Random2(0, 100);
    m_seedZ = Random2(0, 100);
    m_SceneChange = false;
    m_randomSeed = true;
    PerlinNoise perlin;

    for (int z = 0; z < fieldSizeZ; z++)
    {
        for (int x = 0; x < fieldSizeX; x++)
        {
            grassBlock[z * fieldSizeX + x] = AddGameObject<GrassBlock>(1);

            float xSample = (x + m_seedX) / m_bump;
            float zSample = (z + m_seedZ) / m_bump;
            float noise = perlin.noise(xSample, zSample);

            float y = 0.0f;
            y = m_maxHeight * noise;

            grassBlock[z * fieldSizeX + x]->SetPosition(D3DXVECTOR3(x, (int)y, z));
        }
    }

    Fade* fade = AddGameObject<Fade>(2);
    fade->FadeIn();

    wipe = AddGameObject<Wipe>(2);

    // インタラクティブミュージック(縦の遷移)
	m_BGM1 = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM1->Load("asset\\sound\\BGM1.wav");
	m_BGM1->Play(0.05f, true);

	m_BGM2 = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM2->Load("asset\\sound\\BGM2.wav");
	m_BGM2->Play(0.00f, true);   

}

void Title::Uninit()
{
	Scene::Uninit();

    GrassBlock().Unload();
    Simple3d().Unload();
}

void Title::Update()
{
	Scene::Update();

    if (m_SceneChange == true)return;

    // シーンを切り替える処理
    if (Input::GetKeyPress(VK_SPACE))
    {
        m_SceneChange = true;
        Manager::SetScene<Game>();

        //BGMフェードイン、SE再生、画面フェードイン
        SceneChange();
    }

    // 地形を変更する処理
    PerlinNoise perlin;
    for (int z = 0; z < fieldSizeZ; z++)
    {
        for (int x = 0; x < fieldSizeX; x++)
        {
            float xSample = (x + m_seedX) / m_bump;
            float zSample = (z + m_seedZ) / m_bump;
            float noise = perlin.noise(xSample, zSample);

            float y = 0.0f;
            y = m_maxHeight * noise;

            grassBlock[z * fieldSizeX + x]->SetPosition(D3DXVECTOR3(x, (int)y, z));
        }
    }

    // BGMをインタラクティブに切り替える
	if (Input::GetKeyTrigger('A'))
	{
        ChangeBGM(m_BGM1);

		//m_BGM1->SetVolume(0.0f);
		//m_BGM2->SetVolume(0.05f);
	}

	if (Input::GetKeyTrigger('D'))
	{
		m_BGM1->SetVolume(0.05f);
		m_BGM2->SetVolume(0.0f);
	}
}

void Title::SceneChange()
{
    
}

void Title::ChangeBGM(Audio* audio1)
{
    float i = 0.05f;

    while (audio1->GetVolume() > 0.0f)
    {
        audio1->SetVolume(i);
        i -= 0.00000001f;
    }
}