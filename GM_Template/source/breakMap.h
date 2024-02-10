#pragma once
#include"gameObject.h"

class BreakMap : public GameObject
{
private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11InputLayout*			m_VertexLayout{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11VertexShader*			m_VertexShader{};
	
	int m_BreakMap{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddBreakMap(int BreakMap) { m_BreakMap += BreakMap; }
	void SetBreakMap(int BreakMap) { m_BreakMap = BreakMap; }
	int GetBreakMap() { return m_BreakMap; }
};