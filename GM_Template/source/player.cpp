#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "player.h"
#include "coin.h"
#include "enemy.h"
#include "simple3d.h"
#include "warpBlock.h"
#include "footSmoke.h"
#include "shadow.h"
#include <XInput.h>


#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define GRAVITY 0.01f
#define SIZE 0.8f


void Player::Init()
{
	GameObject::Init();

	m_Model = new Model();
	m_Model->Load("asset\\model\\Player.obj");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.5f, 0.9f, 0.5f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	m_Speed = 0.01f;
	m_GroundHeight = 0.0f;
	m_GroundHeightTemp = 0.0f;
	m_IsEnable = true;
	m_PlayerCollision = NONE;

	m_Shadow = AddComponent<Shadow>();
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

	m_PlayerCollision = NONE;

	m_GroundHeight = 0.0f;
	m_GroundHeightTemp = 0.0f;
	m_OldPosition = m_Position;
	m_Velocity.x = 0.0f;
	m_Velocity.z = 0.0f;

	// �L�[�{�[�h����
	if (Input::GetKeyPress('A') || Input::GetKeyPress('D'))
	{
		if (Input::GetKeyPress('W') || Input::GetKeyPress('S'))
		{
			m_Speed = 0.035f;
		}
		else
		{
			m_Speed = 0.05f;
		}
	}
	else if (Input::GetKeyPress('W') || Input::GetKeyPress('S'))
	{
		m_Speed = 0.05f;
	}

	if (Input::GetKeyPress('W'))
	{
		m_Velocity.z = m_Speed;

		// ���s�G�t�F�N�g
		if (!scene->GetGameObject<FootSmoke>())
		{
			D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.25f, -m_Scale.z/2);
			scene->AddGameObject<FootSmoke>(1)->SetPosition(m_Position + offset);
		}
	}
	if (Input::GetKeyPress('A'))
	{
		m_Velocity.x = -m_Speed;

		// ���s�G�t�F�N�g
		if (!scene->GetGameObject<FootSmoke>())
		{
			D3DXVECTOR3 offset = D3DXVECTOR3(m_Scale.x/2, 0.25f, 0.0f);
			scene->AddGameObject<FootSmoke>(1)->SetPosition(m_Position + offset);
		}
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity.z = -m_Speed;

		// ���s�G�t�F�N�g
		if (!scene->GetGameObject<FootSmoke>())
		{
			D3DXVECTOR3 offset = D3DXVECTOR3(0.0f, 0.25f, m_Scale.z/2);
			scene->AddGameObject<FootSmoke>(1)->SetPosition(m_Position + offset);
		}
	}
	if (Input::GetKeyPress('D'))
	{
		m_Velocity.x = m_Speed;

		// ���s�G�t�F�N�g
		if (!scene->GetGameObject<FootSmoke>())
		{
			D3DXVECTOR3 offset = D3DXVECTOR3(-m_Scale.x/2, 0.25f, 0.0f);
			scene->AddGameObject<FootSmoke>(1)->SetPosition(m_Position + offset);
		}
	}

	// �R���g���[���[����
	XINPUT_STATE state;
	XINPUT_KEYSTROKE key;
	XInputGetState(0, &state);
	XInputGetKeystroke(0, XINPUT_FLAG_GAMEPAD, &key);

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		// ���l�ݒ�
		if ((state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}

		// �΂߈ړ���x������
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

		//�X�e�B�b�N��O�ɌX������O�Ɉړ�
		if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.z = m_Speed;
		}
		//�X�e�B�b�N����ɌX�������Ɉړ�
		if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.z = -m_Speed;
		}
		// �X�e�B�b�N�����ɌX�����獶�Ɉړ�
		if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.x = -m_Speed;
		}
		// �X�e�B�b�N���E�ɌX������E�Ɉړ�
		if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			m_Velocity.x = m_Speed;
		}
	}

	m_Velocity.y -= GRAVITY;
	m_Position += m_Velocity;

	// �Փ˔���
	CollisionUpdate();

	// �W�����v
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

	// �v���C���[�̌�����ύX
	D3DXVECTOR3 toLookAt = -m_Velocity;
	D3DXVec3Normalize(&toLookAt, &toLookAt);
	m_Rotation.y = atan2f(toLookAt.x, toLookAt.z);

	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = m_GroundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
}

void Player::Draw()
{
	GameObject::Draw();

	if (m_IsEnable == false) return;

	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x,m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

// �����蔻��S�ʂ̏���
void Player::CollisionUpdate()
{
	Scene* scene = Manager::GetScene();

	// Simple3d----------------------------------------------------------------
	auto boxs = scene->GetGameObjects<Simple3d>();    //���X�g���擾
	for (Simple3d* box : boxs)                        //�͈�for���[�v
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();
		D3DXVECTOR3 right = box->GetRight();            //x������
		D3DXVECTOR3 forward = box->GetForward();        //z������
		D3DXVECTOR3 up = box->GetUp();                  //y������
		D3DXVECTOR3 direction = m_Position - position;  //�����̂���v���C���[�܂ł̕����x�N�g��
		float obbx = D3DXVec3Dot(&direction, &right);   //X�����������v���C���[����
		float obbz = D3DXVec3Dot(&direction, &forward); //Z�����������v���C���[����
		float obby = D3DXVec3Dot(&direction, &up);      //Y�����������v���C���[����

		//OBB
		if (fabs(obbx) < SIZE && fabs(obbz) < SIZE && fabs(obby) < SIZE)
		{
			D3DXVECTOR3 penetration = D3DXVECTOR3(SIZE - abs(obbx), SIZE - abs(obby), SIZE - abs(obbz));

			if (penetration.x < penetration.z && penetration.x < penetration.y)
			{
				if (obbx > 0) { m_Position += penetration.x * right; }
				else { m_Position -= penetration.x * right; }
			}
			else if (penetration.z < penetration.y)
			{
				if (obbz > 0) { m_Position += penetration.z * forward; }
				else { m_Position -= penetration.z * forward; }
			}
			else
			{
				if (obby > 0)
				{
					m_Position += penetration.y * up;
					m_Velocity.y = 0.0f;            //��ɏ�����琂�����x��0�ɂ���

					if (Input::GetKeyTrigger(VK_SPACE))
					{
						m_Velocity.y = 0.25f;
					}
				}
				else
				{
					m_Position -= penetration.y * up;
					m_Velocity.y = -m_Velocity.y;    //������G�ꂽ�琂�����x�𔽓]����
				}
			}
		}
	}

	// coin----------------------------------------------------------------
	auto coins = scene->GetGameObjects<Coin>();
	for (Coin* coin : coins)
	{
		D3DXVECTOR3 position = coin->GetPosition();
		D3DXVECTOR3 scale = coin->GetScale();
		D3DXVECTOR3 right = coin->GetRight();
		D3DXVECTOR3 forward = coin->GetForward();
		D3DXVECTOR3 direction = m_Position - position;
		float abbx = D3DXVec3Dot(&direction, &right);
		float abbz = D3DXVec3Dot(&direction, &forward);

		//OBB
		if (fabs(abbx) < scale.x/2 && fabs(abbz) < scale.z/2)
		{
			if (m_Position.y < position.y + scale.y/2)
			{
				coin->SetDestroy();
			}
		}
	}

	// enemy----------------------------------------------------------------
	auto enemys = scene->GetGameObjects<Enemy>();
	for (Enemy* enemy : enemys)
	{
		D3DXVECTOR3 position = enemy->GetPosition();
		D3DXVECTOR3 scale = enemy->GetScale();
		D3DXVECTOR3 right = enemy->GetRight();
		D3DXVECTOR3 forward = enemy->GetForward();
		D3DXVECTOR3 direction = m_Position - position;
		float abbx = D3DXVec3Dot(&direction, &right);
		float abbz = D3DXVec3Dot(&direction, &forward);

		//OBB
		if (fabs(abbx) < scale.x && fabs(abbz) < scale.z)
		{
			if (m_Position.y < position.y + scale.y)
			{
				m_PlayerCollision = ENEMY;
			}
		}
	}

	// warpBlock----------------------------------------------------------------
	auto warpBlocks = scene->GetGameObjects<WarpBlock>();
	for (WarpBlock* warpBlock : warpBlocks)
	{
		D3DXVECTOR3 position	= warpBlock->GetPosition();
		D3DXVECTOR3 scale		= warpBlock->GetScale();
		D3DXVECTOR3 right		= warpBlock->GetRight();
		D3DXVECTOR3 forward		= warpBlock->GetForward();
		D3DXVECTOR3 direction	= m_Position - position;
		float abbx = D3DXVec3Dot(&direction, &right);
		float abbz = D3DXVec3Dot(&direction, &forward);

		//OBB
		if (fabs(abbx) < scale.x && fabs(abbz) < scale.z)
		{
			if (m_Position.y < position.y + scale.y)
			{
				m_PlayerCollision = WARP_BLOCK;
			}
		}
	}
}