#pragma once
#include"gameObject.h"

enum struct WIPW_STATE
{
	NONE,
	OPEN,
	CLOSE
};

class Wipe : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//ここに	シェーダー関連の変数を追加
	ID3D11VertexShader*	m_VertexShader;
	ID3D11PixelShader*	m_PixelShader;
	ID3D11InputLayout*	m_VertexLayout;

	WIPW_STATE m_WipeState;

	class Sprite* m_Sprite{};

	float m_Threshold;
	bool m_Wipe;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ChangeState(WIPW_STATE ws) { m_WipeState = ws; }
};