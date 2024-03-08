#pragma once

#include "model.h"
#include "gameObject.h"

/// <summary>
/// エネミークラス(チェイス)
/// </summary>
class Chase : public GameObject
{
private:
	struct OLDPOS
	{
		D3DXVECTOR3 pos;
	};

	static Model* m_Model;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader*	m_PixelShader{};
	ID3D11InputLayout*	m_VertexLayout{};

	OLDPOS oldpos[99];
	int m_Frame;
	int m_Frame2;
	bool on;

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

};