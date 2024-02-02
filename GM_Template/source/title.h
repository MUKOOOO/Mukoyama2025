#pragma once
#include "scene.h"

#define fieldSizeX 50
#define fieldSizeZ 50

class Title:public Scene
{
private:
	class Camera* camera{};
	class Audio* m_BGM1{};
	class Audio* m_BGM2{};
	class GrassBlock* grassBlock[fieldSizeX*fieldSizeZ]{};

	static float m_bump;
	static float m_maxHeight;
	float m_seedX;
	float m_seedZ;

	bool m_SceneChange;
	bool m_randomSeed;

public:
	void Init();
	void Uninit();
	void Update();

	void SceneChange();

	void SetBump(float bump) { m_bump = bump; }
	float GetBump() { return m_bump; }

	void SetMaxHeight(float maxHeight) { m_maxHeight = maxHeight; }
	float GetMaxHeight() { return m_maxHeight; }

	void ChangeBGM(Audio* audio1);
};