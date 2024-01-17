#pragma once

#include "model.h"
#include"gameObject.h"

class Player : public GameObject
{
private:
	Model* m_Model{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};

	float m_Speed;
	float m_GroundHeight;
	float m_GroundHeightTemp;

	bool m_IsEnable;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetEnable(bool flg) { m_IsEnable = flg; }
};