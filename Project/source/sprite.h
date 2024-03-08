#pragma once
#include"component.h"

/// <summary>
/// テクスチャを描画するコンポーネント
/// </summary>
class Sprite : public Component
{
private:
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ID3D11Buffer*			  m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
public:
	void Init(float x, float y, float width, float height, const char* TextureName);
	void Uninit();
	void Update();
	void Draw();

	void DrawMove(float x, float y, float width, float height);
	void SetColor(D3DXCOLOR Color) {m_Color = Color;}
};