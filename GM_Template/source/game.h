#pragma once
#include "scene.h"
#include <random>

#define DEFAULT_BLOCK 32

class Game:public Scene
{
private:
	class Simple3d* block[DEFAULT_BLOCK]{};

	D3DXVECTOR3 BlockPosition[DEFAULT_BLOCK] = {
		{4.0f,0.0f,4.0f},
		{4.0f,2.0f,4.0f},
		{10.0f,0.0f,4.0f},
		{10.0f,2.0f,4.0f},
		{16.0f,0.0f,4.0f},
		{16.0f,2.0f,4.0f},
		{22.0f,0.0f,4.0f},
		{22.0f,2.0f,4.0f},
		{4.0f,0.0f,10.0f},
		{4.0f,2.0f,10.0f} 
	};

	int m_Frame;

public:
	void Init();
	void Uninit();
	void Update();

	int GetFrame() { return m_Frame; }
};