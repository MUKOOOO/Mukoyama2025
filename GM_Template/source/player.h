#pragma once

#include "model.h"
#include"gameObject.h"

 enum PlayerCollision
{
	NONE,
	WARP_BLOCK,
	ENEMY
};

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

	class Shadow* m_Shadow{};

	PlayerCollision m_PlayerCollision = NONE;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void CollisionUpdate();
	void SetEnable(bool flg) { m_IsEnable = flg; }
	PlayerCollision GetPlayerCollision() { return m_PlayerCollision; }
	void SetPlayerCollision(PlayerCollision pc) { m_PlayerCollision = pc; }
};