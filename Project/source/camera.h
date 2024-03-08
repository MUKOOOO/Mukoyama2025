#pragma once
#include"gameObject.h"

class  Camera : public GameObject
{
private:
	D3DXMATRIX	m_ViewMatrix{};
	D3DXMATRIX	m_ProjectionMateix{};

	D3DXVECTOR3	m_Target{};
	D3DXVECTOR3	m_Angle{};

	bool m_CameraAnime;
	float m_Distance;	//�J��������
	float m_Height;		//���������̉�]
	float m_Rot;			//���������̉�]
public:
	void Init();
	void Update();
	void Draw();

	// �J�����O����
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};