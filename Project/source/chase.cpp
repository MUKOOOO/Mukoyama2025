#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "chase.h"
#include "camera.h"
#include "player.h"

Model* Chase::m_Model{};

void Chase::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\block07.obj");
}

void Chase::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Chase::Init()
{
	m_Position = D3DXVECTOR3(13.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Frame = 0;
	m_Frame2 = 0;
	on = false;
}

void Chase::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Chase::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();

	oldpos[m_Frame].pos = scene->GetGameObject<Player>()->GetPosition();

	m_Frame++;

	if (m_Frame >= 99)m_Frame = 0;

	if (m_Frame > 45)
	{
		on = true;
		
	}
	if (on == true)
	{
		m_Position = oldpos[m_Frame2].pos;
		m_Frame2++;
		if (m_Frame2 >= 99)
		{
			m_Frame2 = 0;
		}
	}
}

void Chase::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

	GameObject::Draw();

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}