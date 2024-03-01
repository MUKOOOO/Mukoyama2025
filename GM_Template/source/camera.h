#pragma once
#include"gameObject.h"

class  Camera : public GameObject
{
private:
	D3DXMATRIX	m_ViewMatrix{};
	D3DXMATRIX	m_ProjectionMateix{};

	D3DXVECTOR3	m_Target{};
	D3DXVECTOR3	m_Angle{};

	float m_Distance;	//カメラ距離
	float m_Height;		//垂直方向の回転
	float m_Rot;		//水平方向の回転
public:
	void Init();
	void Update();
	void Draw();

	// カリング処理
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};