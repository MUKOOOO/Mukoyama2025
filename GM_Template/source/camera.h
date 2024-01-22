#pragma once
#include"gameObject.h"

enum CAMERA_TYPE
{
	Normal,
	LookingDown,
	Follow
};

class  Camera : public GameObject
{
private:
	D3DXVECTOR3 m_Target{};
	D3DXVECTOR3 m_Angle{};
	D3DXMATRIX	m_ViewMatrix{};
	D3DXMATRIX	m_ProjectionMateix{};

	CAMERA_TYPE m_CameraType;

	bool m_SetUp;
public:
	void Init();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
	void ChangeCameraType(CAMERA_TYPE type) { m_CameraType = type; }
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);
};