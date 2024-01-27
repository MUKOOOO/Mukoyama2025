#include "main.h"
#include "renderer.h"
#include "fade.h"
#include"sprite.h"

#define SPEED 0.02f

void Fade::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\fade_white.png");

	m_Alpha = 1.0f;
	m_FadeIn = false;
	m_FadeOut = false;
	m_FadeEnabled = false;

	FadeIn();
}

void Fade::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Fade::Update()
{
	GameObject::Update();

	//フェードイン(徐々に画面が見えてくる)
	if (m_FadeIn == true)
	{
		m_Alpha -= SPEED;
	}
	
	//フェードイン(徐々に画面が見えなくなる)
	if (m_FadeOut == true)
	{
		m_Alpha += SPEED;
	}
	
	//α値に応じてフェードアウトの処理を終了
	if (m_Alpha > 1.0f)
	{
		m_Alpha = 1.0f;
		m_FadeOut = false;
		m_FadeEnabled = false;
	}

	//α値に応じてフェードインの処理を終了
	if (m_Alpha < 0.0f)
	{
		m_Alpha = 0.0f;
		m_FadeIn = false;
		m_FadeEnabled = false;
	}
	
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}

void Fade::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	GameObject::Draw();
}

void Fade::FadeIn()
{
	//もしフェードしていなければフェードインする
	if (m_FadeEnabled == false)
	{
		m_FadeEnabled = true;
		m_FadeIn = true;
	}
}

void Fade::FadeOut()
{
	//もしフェードしていなければフェードアウトする
	if (m_FadeEnabled == false)
	{
		m_FadeEnabled = true;
		m_FadeOut = true;
	}
}