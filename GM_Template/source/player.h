#pragma once

#include "model.h"
#include"gameObject.h"

 enum PlayerCollision
{
	NONE,
	COIN,
	ENEMY,
	WARP_BLOCK,
};

class Player : public GameObject
{
private:
	class Audio* m_CoinSE{};
	class Audio* m_FootSE{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_VelocityTemp{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	Model* m_Model{};

	PlayerCollision m_PlayerCollision = NONE;

	float m_Speed;
	float m_GroundHeight;
	float m_GroundHeightTemp;

	bool m_IsEnable;

	class Shadow* m_Shadow{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void CollisionUpdate();
	void SetEnable(bool flg) { m_IsEnable = flg; }
	PlayerCollision GetPlayerCollision() { return m_PlayerCollision; }
	void SetPlayerCollision(PlayerCollision collision) { m_PlayerCollision = collision; }
};