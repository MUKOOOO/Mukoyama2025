#pragma once
#include "model.h"
#include "gameObject.h"

/// <summary>
/// �I�u�W�F�N�g��`�悷��
/// </summary>
class Object : public GameObject
{
private:
	static Model* m_Model;

	ID3D11PixelShader* m_PixelShader{};
	ID3D11VertexShader* m_VertexShader{};
	ID3D11InputLayout*	m_VertexLayout{};

public:
	static void Load(const char* ObjectName);
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};