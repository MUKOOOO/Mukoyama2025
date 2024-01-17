#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "imgui/imguiManager.h"
#include "scene.h"
#include "input.h"
#include "game.h"

//静的メンバ変数は再宣言が必要
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	ImguiManager::Init();
	Input::Init();

	SetScene<Game>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Input::Uninit();
	ImguiManager::Uninit();
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
	ImguiManager::Begin();

	m_Scene->Draw();
	
	ImguiManager::End();
	Renderer::End();
}