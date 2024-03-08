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
	float m_Distance;	//ƒJƒƒ‰‹——£
	float m_Height;		//‚’¼•ûŒü‚Ì‰ñ“]
	float m_Rot;			//…•½•ûŒü‚Ì‰ñ“]
public:
	void Init();
	void Update();
	void Draw();

	// ƒJƒŠƒ“ƒOˆ—
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);

	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }
};