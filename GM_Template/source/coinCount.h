#pragma once
#include"gameObject.h"

enum class  CoinCountState
{
	NONE,
	UP,
	DOWN
};

class CoinCount : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11InputLayout*			m_VertexLayout{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11VertexShader*			m_VertexShader{};
	
	D3DXVECTOR2 m_pos{};

	int m_Coin{};

	CoinCountState m_CoinCountState = CoinCountState::NONE;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddCoin(int BreakMap);
	int GetCoin() { return m_Coin; }
};