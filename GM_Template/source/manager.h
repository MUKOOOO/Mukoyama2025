#pragma once
#include "scene.h"
//class Scene;

class Manager
{	
private:
	static Scene* m_Scene;
	static Scene* m_NextScene;
	static int m_SScore;

public:
	
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}
	void SetScore(int a) { m_SScore = a; }
	int GetScore(void) { return m_SScore; }
};