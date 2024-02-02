#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "imgui/imguiManager.h"
#include "scene.h"
#include "audio.h"
#include "input.h"
#include "title.h"
#include "game.h"

//静的メンバ変数は再宣言が必要
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
#ifndef Debag
	ImguiManager::Init();
#endif // !Debag
	Input::Init();
	Audio::InitMaster();

	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster;
	Input::Uninit();
#ifndef Debag
	ImguiManager::Uninit();
#endif // !Debag
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (m_NextScene)
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}

	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

#ifndef Debag
	ImguiManager::Begin();
#endif // !Debag

	m_Scene->Draw();
	
#ifndef Debag
	ImguiManager::End();
#endif // !Debag
	Renderer::End();
}