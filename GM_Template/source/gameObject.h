#pragma once
#include"component.h"
#include <list>

class GameObject
{

protected:
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_OldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Component;

public:
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetPosition(D3DXVECTOR3 Position)
	{
		m_Position = Position;
	}

	void SetRotation(D3DXVECTOR3 Rotation)
	{
		m_Rotation = Rotation;
	}

	void SetScale(D3DXVECTOR3 Scale)
	{
		m_Scale = Scale;
	}

	D3DXVECTOR3 GetPosition()
	{
		return m_Position;
	}

	D3DXVECTOR3 GetScale()
	{
		return m_Scale;
	}

	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;

		return forward;
	}

	D3DXVECTOR3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;
		return up;
	}

	virtual void Init() 
	{
		
	};
	virtual void Uninit()
	{
		for (Component* component : m_Component)
		{
			component->Uninit();
		}

		m_Component.clear();
	};
	virtual void Update()
	{
		for (Component* component : m_Component)
		{
			component->Update();
		}
	};
	virtual void Draw()
	{
		for (Component* component : m_Component)
		{
			component->Draw();
		}
	};

	template <typename T>
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		return (T*)component;
	}
};