#pragma once
#include "scene.h"

/// <summary>
/// タイトルシーンを管理する
/// </summary>
class Title:public Scene
{
private:
	class Audio*		m_Music{};
	class Audio*		m_SoundEffect{};
	class Camera*	m_Camera{};
	class Object*	m_Object{};
	class Wipe*		m_Wipe{};

	bool m_SceneChange;
	int m_Frame;
public:
	void Init();
	void Uninit();
	void Update();
};