#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "simple3d.h"
#include "hitEffect.h"
#include <XInput.h>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

#define GRAVITY 0.01f

int interval = 0;

void Player::Init()
{
	GameObject::Init();

	m_Model = new Model();
	m_Model->Load("asset\\model\\PRT_Player.obj");

	m_Position = D3DXVECTOR3(26.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Speed = 0.1f;
	m_GroundHeight = 0.0f;
	m_GroundHeightTemp = 0.0f;
	m_IsEnable = true;
}

void Player::Uninit()
{
	GameObject::Uninit();

	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	GameObject::Update();

	if (m_IsEnable == false) return;

	Scene* scene = Manager::GetScene();

	m_GroundHeight = 0.0f;
	m_GroundHeightTemp = 0.0f;
	m_OldPosition = m_Position;
	m_Velocity.x = 0.0f;
	m_Velocity.z = 0.0f;

	// キーボード操作
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('W') || Input::GetKeyPress('S'))
		{
			m_Speed = 0.07f;
		}
		else
		{
			m_Speed = 0.1f;
		}
	}
	else if (Input::GetKeyPress('W') || Input::GetKeyPress('S'))
	{
		m_Speed = 0.1f;
	}

	if (Input::GetKeyPress('W'))
	{
		m_Velocity.z = m_Speed;
	}
	if (Input::GetKeyPress('A'))
	{
		m_Velocity.x = -m_Speed;

		if (interval == 0)
		{
			D3DXVECTOR3 offset = D3DXVECTOR3(m_Scale.x, 0.25f, 0.0f);
			scene->AddGameObject<HitEffect>(1)->SetPosition(m_Position + offset);
			interval++;
		}
		else
		{
			interval++;

			if (interval >= 12)interval = 0;
		}
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity.z = -m_Speed;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity.x = m_Speed;
	}

	// コントローラー操作
	XINPUT_STATE state;
	XINPUT_KEYSTROKE key;
	XInputGetState(0, &state);
	XInputGetKeystroke(0, XINPUT_FLAG_GAMEPAD, &key);

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		// 闘値設定
		if ((state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}

		// 斜め移動を遅くする
		if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
				state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				m_Speed = 0.07f;
			}
			else
			{
				m_Speed = 0.1f;
			}
		}
		else if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ||
			state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Speed = 0.1f;
		}

		//スティックを前に傾けたら前に移動
		if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.z = m_Speed;
		}
		//スティックを後に傾けたら後に移動
		if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.z = -m_Speed;
		}
		// スティックを左に傾けたら左に移動
		if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.x = -m_Speed;
		}
		// スティックを右に傾けたら右に移動
		if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.x = m_Speed;
		}
	}

	m_Velocity.y -= GRAVITY;
	m_Position += m_Velocity;

	// 衝突判定の処理
	auto boxs = scene->GetGameObjects<Simple3d>();//リストを取得
	for (Simple3d* box : boxs)//範囲forループ
	{
		D3DXVECTOR3 position  = box->GetPosition();
		D3DXVECTOR3 scale	  = box->GetScale();
		D3DXVECTOR3 right	  = box->GetRight();        //x軸分離
		D3DXVECTOR3 forward	  = box->GetForward();      //z軸分離
		D3DXVECTOR3 direction = m_Position - position;  //直方体からプレイヤーまでの方向ベクトル
		float abbx = D3DXVec3Dot(&direction, &right);   //X軸分離方向プレイヤー距離
		float abbz = D3DXVec3Dot(&direction, &forward); //X軸分離方向プレイヤー距離

		//OBB
		if (fabs(abbx) < scale.x * 2.0f && fabs(abbz) < scale.z * 2.0f)
		{	
			if (m_Position.y < position.y + scale.y)
			{
				m_Position.x = m_OldPosition.x;
				m_Position.z = m_OldPosition.z;
			}
			else 
			{
				m_GroundHeightTemp = position.y + scale.y * 2.0f;

				if (m_GroundHeight < m_GroundHeightTemp)
				{
					m_GroundHeight = m_GroundHeightTemp;
				}
			}
		}
	}

	// ジャンプ
	if (m_Position.y <= m_GroundHeight && m_Velocity.y <= 0.0f)
	{
		m_Position.y = m_GroundHeight;
		m_Velocity.y = 0.0f;

		if (Input::GetKeyTrigger(VK_SPACE))
		{
			m_Velocity.y = 0.25f;
		}

		if (key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			m_Velocity.y = 0.25f;
		}
	}

	// プレイヤーの向きを変更
	D3DXVECTOR3 toLookAt = -m_Velocity;
	D3DXVec3Normalize(&toLookAt, &toLookAt);
	m_Rotation.y = atan2f(toLookAt.x, toLookAt.z);
}

void Player::Draw()
{
	GameObject::Draw();

	if (m_IsEnable == false) return;

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