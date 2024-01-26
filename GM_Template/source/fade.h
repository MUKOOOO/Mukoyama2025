#pragma once
#include"gameObject.h"

//フェードアウト→徐々に画面が見えなくなる
//フェードイン→徐々に画面が見えるようになる
class Fade : public GameObject
{
private:
	ID3D11VertexShader*	m_VertexShader{};
	ID3D11PixelShader*	m_PixelShader{};
	ID3D11InputLayout*	m_VertexLayout{};

	class Sprite* m_Sprite{};

	float	m_Alpha;
	bool		m_FadeIn;
	bool		m_FadeOut;
	bool		m_FadeEnabled;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void		FadeOut();
	void		FadeIn();
	bool		GetFadeEnabled() { return m_FadeEnabled; }

};