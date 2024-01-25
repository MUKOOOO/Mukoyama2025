#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "title.h"
#include "perlinNoise.h"
#include "input.h"
#include "camera.h"
#include "sky.h"
#include "field.h"
#include "simple3d.h"

std::random_device rd2;
std::mt19937 gen2(rd2());

int Random2(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen2);
}

void Title::Init()
{
	m_SceneNumber = 1;

    Simple3d().Load();

	camera = AddGameObject<Camera>(0);
	camera->SetPosition(D3DXVECTOR3(6.0f, 18.0f, -16.0f));
	camera->SetTarget(D3DXVECTOR3(6.0f, 1.0f, 0.0f));

	AddGameObject<Sky>(1);
	//AddGameObject<Field>(1);

	m_BGM1 = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM1->Load("asset\\sound\\BGM1.wav");
	m_BGM1->Play(0.05f, true);

	m_BGM2 = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM2->Load("asset\\sound\\BGM2.wav");
	m_BGM2->Play(0.00f, true);

    Simple3d* box[2500];
    
    //パーリンノイズ実験2
    PerlinNoise perlin;
    float _seedX = Random2(0, 100);
    float _seedz = Random2(0, 100);

    for (int x = 0; x < 25; x++)
    {
        for (int z = 0; z < 25; z++)
        {
            box[x] = AddGameObject<Simple3d>(1);

            float xSample = (x + _seedX) / 10.0f;
            float zSample = (z + _seedz) / 10.0f;

            float noise = perlin.noise(xSample, zSample);

            float y = 0.0f;
            y = 4.0f * noise;
            y = (int)y;

            box[x]->SetPosition(D3DXVECTOR3(x, y, z));
        }
    }
}

void Title::Uninit()
{
	Scene::Uninit();

    Simple3d().Unload();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger('A'))
	{
		m_BGM1->VolumeSet(0.0f);
		m_BGM2->VolumeSet(0.05f);
	}

	if (Input::GetKeyTrigger('D'))
	{
		m_BGM1->VolumeSet(0.05f);
		m_BGM2->VolumeSet(0.0f);
	}
}