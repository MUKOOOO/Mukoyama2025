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
#include <time.h>

void Title::Init()
{
    // 現在のシーンを設定
    m_sceneState = SCENE_STATE::TITLE;

    // モデル読み込み
    GrassBlock().Load();

    // オブジェクトの配置
	m_Camera = AddGameObject<Camera>(0);
	m_Camera->SetPosition(D3DXVECTOR3(12.0f, 5.05f, -6.0f));

    m_Object = AddGameObject<Object>(1);
    m_Object->Load("asset\\model\\TitleLogo.obj");
    m_Object->SetPosition(D3DXVECTOR3(0.0f, 2.0f, 0.0f));

    AddGameObject<Sky>(1);

    Fade* fade = AddGameObject<Fade>(2);
    fade->FadeIn();

    m_Wipe = AddGameObject<Wipe>(2);
    m_Wipe->SetThreshold(1.0f);

	m_Music = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_Music->Load("asset\\sound\\BGM1.wav");
	m_Music->Play(0.05f, true);

    // メンバ変数の初期化
    m_SceneChange = false;
    m_Frame = 0;
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

    // シーン遷移
    if (m_SceneChange == true)
    {
        if (m_Frame == 100)//ワイプ処理
        {
            m_Wipe->ChangeState(WIPW_STATE::CLOSE);
        }
        if (m_Frame == 200)//シーン切り替え
        {
            Manager::SetScene<Game>();
        }
        m_Frame++;
        return;
    }

    if (Input::GetKeyPress(VK_SPACE))
    {
        m_SceneChange = true;
        m_Music->Stop();
    }
}