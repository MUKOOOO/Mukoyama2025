#pragma once

/// <summary>
/// Imgui���Ǘ����邽�߂̃N���X
/// </summary>
class  ImguiManager
{
private:
	static char m_Buffer[1024];
	static bool flg;
	static float vec4a[3];
	static float m_Rotation[3];
	static float m_Scale[3];
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();
};