#pragma once

/// <summary>
/// Imguiを管理するクラス
/// </summary>
class  ImguiManager
{
private:
	static char m_Buffer[1024];
	static bool flg;
	static bool randomSeed;
	static float vec4a[3];
	static float m_Rotation[3];
	static float m_Scale[3];
	static float m_Bump;
	static float m_MaxHeight;
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void TitleScene();
	static void StageScene();
};