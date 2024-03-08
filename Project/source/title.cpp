#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "title.h"
#include "game.h"
#include "input.h"
#include "audio.h"
#include "camera.h"
#include "fade.h"
#include "grassBlock.h"
#include "object.h"
#include "sky.h"
#include "wipe.h"

void Title::Init()
{
    // �V�[�����^�C�g���ɕύX
    m_sceneState = SCENE_STATE::TITLE;

    // ���f���ǂݍ���
    GrassBlock().Load();

    // �I�u�W�F�N�g�̔z�u
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetPosition(D3DXVECTOR3(12.0f, 5.05f, -6.0f));

    m_Object = AddGameObject<Object>(1);
    m_Object->Load("asset\\model\\TitleLogo.obj");
    m_Object->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));

    AddGameObject<Sky>(1);

    Fade* fade = AddGameObject<Fade>(2);
    fade->FadeIn();

    m_Wipe = AddGameObject<Wipe>(2);

	m_Music = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_Music->Load("asset\\sound\\BGM1.wav");
	m_Music->Play(0.05f, true);
}

void Title::Uninit()
{
	Scene::Uninit();

    m_Object->Unload();
    GrassBlock().Unload();
}

void Title::Update()
{
	Scene::Update();

    if (m_SceneChange == true)return;

    // �V�[����؂�ւ��鏈��
    if (Input::GetKeyPress(VK_SPACE))
    {
        m_SceneChange = true;
        Manager::SetScene<Game>();

        //BGM�t�F�[�h�C���ASE�Đ��A��ʃt�F�[�h�C��
        SceneChange();
    }
}

void Title::SceneChange()
{
    
}