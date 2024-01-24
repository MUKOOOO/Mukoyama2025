#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "imgui/imguiManager.h"
#include "scene.h"
#include "audio.h"
#include "input.h"
#include "title.h"
#include "game.h"

//�ÓI�����o�ϐ��͍Đ錾���K�v
Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	ImguiManager::Init();
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