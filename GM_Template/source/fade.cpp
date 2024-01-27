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

	//�t�F�[�h�C��(���X�ɉ�ʂ������Ă���)
	if (m_FadeIn == true)
	{
		m_Alpha -= SPEED;
	}
	
	//�t�F�[�h�C��(���X�ɉ�ʂ������Ȃ��Ȃ�)
	if (m_FadeOut == true)
	{
		m_Alpha += SPEED;
	}
	
	//���l�ɉ����ăt�F�[�h�A�E�g�̏������I��
	if (m_Alpha > 1.0f)
	{
		m_Alpha = 1.0f;
		m_FadeOut = false;
		m_FadeEnabled = false;
	}

	//���l�ɉ����ăt�F�[�h�C���̏������I��
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
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	GameObject::Draw();
}

void Fade::FadeIn()
{
	//�����t�F�[�h���Ă��Ȃ���΃t�F�[�h�C������
	if (m_FadeEnabled == false)
	{
		m_FadeEnabled = true;
		m_FadeIn = true;
	}
}

void Fade::FadeOut()
{
	//�����t�F�[�h���Ă��Ȃ���΃t�F�[�h�A�E�g����
	if (m_FadeEnabled == false)
	{
		m_FadeEnabled = true;
		m_FadeOut = true;
	}
}