#pragma once
#include "scene.h"

class Title:public Scene
{
private:
	class Camera* camera;
	class Audio* m_BGM1;
	class Audio* m_BGM2;

public:
	void Init();
	void Uninit();
	void Update();
	
};