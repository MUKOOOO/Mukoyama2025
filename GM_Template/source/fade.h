#pragma once
#include"gameObject.h"

//�t�F�[�h�A�E�g�����X�ɉ�ʂ������Ȃ��Ȃ�
//�t�F�[�h�C�������X�ɉ�ʂ�������悤�ɂȂ�
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