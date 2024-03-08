#pragma once
#include"gameObject.h"

// �J�����̏�Ԃ�\���񋓌^
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
	float m_Distance;	//�J��������
	float m_Height;		//���������̉�]
	float m_Rot;			//���������̉�]
public:
	D3DXMATRIX GetViewMatrix() { return m_ViewMatrix; }

	void Init();
	void Update();
	void Draw();

	void DefaultCamera();
	void SetCameraState(CAMERA_STATE camera_state) { m_CameraState = camera_state; }
	void StageCamera();

	// �J�����O����
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);
};