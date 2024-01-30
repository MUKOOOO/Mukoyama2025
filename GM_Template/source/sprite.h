#pragma once
#include"component.h"

class Sprite : public Component
{
private:
	ID3D11Buffer* m_VertexBuffer{};
	ID3D11ShaderResourceView* m_Texture{};
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

public:
	void Init(float x, float y, float width, float height, const char* TextureName);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR Color) {m_Color = Color;}
	void DrawMove(float x, float y, float width, float height);

};