#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "audio.h"
#include "title.h"
#include "input.h"
#include "camera.h"
#include "field.h"

void Title::Init()
{
	m_SceneNumber = 1;

	//AddGameObject<Camera>(0)->ChangeCameraType(Follow);

	//AddGameObject<Field>(1);

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