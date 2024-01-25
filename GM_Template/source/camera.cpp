#include"main.h"
#include"manager.h"
#include"renderer.h"
#include "input.h"
#include "camera.h"
#include"player.h"
#include"scene.h"

#define X_MAX 4.0f
#define X_MIN -4.0f
#define Z_MAX 4.0f
#define Z_MIN -4.0f

#define FOLLOW D3DXVECTOR3(0.0f,12.0f,-4.0f)
#define LOOKINGDOWN D3DXVECTOR3(0.0f,12.0f,-4.0f)

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_CameraType = Normal;

	m_SetUp = false;
}

void Camera::Update()
{	
	Scene* scene = Manager::GetScene();

	// ・タイトルでのカメラ仕様
	// 　矢印キーでX/Z方向移動
	// 　Y方向移動
	// 　マウスドラッグで上下左右の視線移動

	switch (scene->GetSceneName())
	{
	case SCENE_NAME::TITLE:
		switch (m_CameraType)
		{
		case Normal://原点

			break;
		case LookingDown://俯瞰
			if (m_SetUp == false)
			{
				m_SetUp = true;
				m_Angle = LOOKINGDOWN;
			}

			m_Position = m_Target + m_Angle;
			break;
		}

		if (Input::GetKeyPress(VK_UP))
		{
			m_Angle.y += 0.1f;
			m_Angle.z += 0.1f;
		}
		if (Input::GetKeyPress(VK_DOWN))
		{
			m_Angle.y -= 0.1f;
			m_Angle.z -= 0.1f;
		}

		m_Position = m_Target + m_Angle;
		break;
	case SCENE_NAME::STAGE:
		Player* player = scene->GetGameObject<Player>();

		switch (m_CameraType)
		{
		case Normal://原点

			break;
		case LookingDown://俯瞰
			if (m_SetUp == false)
			{
				m_SetUp = true;
				m_Angle = LOOKINGDOWN;
			}

			m_Position = m_Target + m_Angle;

			if (Input::GetKeyPress(VK_UP))
			{
				m_Angle.y += 0.1f;
				m_Angle.z += 0.1f;
			}
			if (Input::GetKeyPress(VK_DOWN))
			{
				m_Angle.y -= 0.1f;
				m_Angle.z -= 0.1f;
			}
			break;
		case Follow://追従
			if (m_SetUp == false)
			{
				m_SetUp = true;
				m_Angle = FOLLOW;
			}

			m_Target = player->GetPosition();
			m_Position = m_Target + m_Angle;

			if (Input::GetKeyPress(VK_UP))
			{
				m_Angle.y += 0.1f;
				m_Angle.z += 0.1f;
			}
			if (Input::GetKeyPress(VK_DOWN))
			{
				m_Angle.y -= 0.1f;
				m_Angle.z -= 0.1f;
			}
			break;
		}
		break;
	}
}

void Camera::Draw()
{
	// ビューマトリクス設定
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);

	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, 
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

bool Camera::CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale)
{
	D3DXMATRIX vp, invvp;
	vp = m_ViewMatrix * m_ProjectionMateix;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vpos[4];
	D3DXVECTOR3 wpos[4];

	// スケールを適用
	vpos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vpos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vpos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vpos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

	// ビューフラスタムの各頂点を透視変換してワールド座標に変換
	D3DXVec3TransformCoord(&wpos[0], &vpos[0], &invvp);
	D3DXVec3TransformCoord(&wpos[1], &vpos[1], &invvp);
	D3DXVec3TransformCoord(&wpos[2], &vpos[2], &invvp);
	D3DXVec3TransformCoord(&wpos[3], &vpos[3], &invvp);

	D3DXVECTOR3 v, v1, v2, n;
	v = Position - m_Position;

	// 左面判定
	v1 = wpos[0] - m_Position;
	v2 = wpos[2] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);
	float distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.x) { return false; }

	// 右面判定
	v1 = wpos[3] - m_Position;
	v2 = wpos[1] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);

	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.x) { return false; }

	// 上面判定
	v1 = wpos[1] - m_Position;
	v2 = wpos[0] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);

	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.z) { return false; }

	// 下面判定
	v1 = wpos[2] - m_Position;
	v2 = wpos[3] - m_Position;
	D3DXVec3Cross(&n, &v1, &v2);
	D3DXVec3Normalize(&n, &n);

	distance = D3DXVec3Dot(&n, &v);
	if (distance < -Scale.z) { return false; }

	return true;
}