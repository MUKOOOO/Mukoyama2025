#include "main.h"
#include "renderer.h"
#include "wipe.h"
#include <iostream>
#include"sprite.h"

void Wipe::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\wipePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\009.jpg");

	m_Threshold = 0.0f;
	m_WipeState = WIPW_STATE::OPEN;
}

void Wipe::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Wipe::Update()
{
	GameObject::Update();

	switch (m_WipeState)
	{
	case WIPW_STATE::NONE:
		break;
	case WIPW_STATE::OPEN:
		if (m_Threshold >= 1.0f)
		{
			m_Threshold = 1.0f;
			m_WipeState = WIPW_STATE::NONE;
		}
		m_Threshold += 0.025f;
		break;
	case WIPW_STATE::CLOSE:
		if (m_Threshold <= -1.0f)
		{
			m_Threshold = -1.0f;
			m_WipeState = WIPW_STATE::NONE;
		}
		m_Threshold -= 0.025f;
		break;
	default:
		break;
	}
}

void Wipe::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = m_Threshold;
	param.dissolveRange = 0.1f;
	Renderer::SetParameter(param);

	GameObject::Draw();
}