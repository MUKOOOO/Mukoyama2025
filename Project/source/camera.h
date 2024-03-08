#pragma once
#include"gameObject.h"

// カメラの状態を表す列挙型
enum struct CAMERA_STATE
{
	Default,
	Stage
};

class  Camera : public GameObject
{
private:
	CAMERA_STATE m_CameraState{};

	D3DXMATRIX	m_ViewMatrix{};
	D3DXMATRIX	m_ProjectionMateix{};

	D3DXVECTOR3	m_Target{};
	D3DXVECTOR3	m_Angle{};

	bool m_CameraAnime;
	float m_Distance;	//カメラ距離
	float m_Height;		//垂直方向の回転
	float m_Rot;			//水平方向の回転
public:
	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

	void Init();
	void Update();
	void Draw();

	void DefaultCamera();
	void SetCameraState(CAMERA_STATE camera_state) { m_CameraState = camera_state; }
	void StageCamera();

	// カリング処理
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);
};